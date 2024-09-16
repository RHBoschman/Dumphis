import * as Filter from './filter.js';

const N_EXTRA_CMNDS = 30; // Should NOT be bigger then number of dumphis items!!!
const REFRESH_VALUE = -1;
const MAX_NR_DATA_CON = 8;

const dumphisPath = '../exports/DumphisData.json';
const unitInfoPath = '../exports/UnitInfoData.json';
const funcDataPath = '../exports/FunctionData.json';

const Columns = Object.freeze({
    TIMESTAMP: 0,
    UNIT: 1,
    COMMAND: 2,
    STEP: 3,
    UNDEF: 99
});

// Global data objects to guarantee access for all code (not pretty, but functional for now)
let dumphisDataObj = null;
let unitInfoDataObj = null;
let functionDataObj = null;
let curFuncIndex = -1;
let curCmndIndex = -1;
let n_createdCmnds = 0, n_createdSteps = 0;
let curUnitName = "";
let maxTimestampIndex = 0;
let curSelectedColumn = Columns.UNDEF;

$(document).ready(function() {
    let selectedTimestamp = 0, newSelTimestamp = 0;
    let selectedUnit = 0, newSelUnit = 0;
    let selectedCmnd = 0, newSelCmnd = 0;
    let selectedStep = 0, newSelStep = 0;

    // Initialize app on button click
    $('#render').on('click', function() {
        Promise.all([readJSON(dumphisPath), readJSON(unitInfoPath), readJSON(funcDataPath)])
            .then(([dumphisData, unitInfoData, funcData]) => {
                dumphisDataObj = dumphisData;
                unitInfoDataObj = unitInfoData;
                functionDataObj = funcData;
                table_addTimestamps();
                table_addUnits();
            })
            .then(() => {
                // Select all first items
                if ($(`#t_${selectedTimestamp}`).length > 0) {
                    $('.timestamp-sel').removeClass('timestamp-sel');
                    $(`#t_${selectedTimestamp}`).addClass('timestamp-sel');
                    selectedTimestamp = REFRESH_VALUE; // To make sure correct unit is selected immediately
                }

                if ($(`#u_${selectedUnit}`).length > 0) {
                    $('.unit-sel').removeClass('unit-sel');
                    $(`#u_${selectedUnit}`).addClass('unit-sel');
                    selectedUnit = REFRESH_VALUE; // To make sure commands are displayed immediately
                }
            })
            .catch(error => {
                console.error('Error fetching JSON:', error);
            });
        $(this).prop('disabled', true);
        $('.disable').prop('disabled', false);
    });

    // Buttons to iterate over timestamps
    $('#next').on('click', function() {
        if (selectedTimestamp < maxTimestampIndex) {
            newSelTimestamp++;
        }
    });

    $('#previous').on('click', function() {
        if (selectedTimestamp > 0) {
            newSelTimestamp--;
        }
    });

    $(document).on('dblclick', '.unit-info-trig', function() {
        let elementId = $(this).closest('td').attr('id');
        if (getFocusedColumn(elementId)) {
            $('#info-volume').empty();
            $('#info-volume').append(createInfoContent(elementId));
        }
        else {
            // TODO: implement error message
        }
        
        // Show info box
        $('#info-box-wrapper').css("display", "block");
        $('#info-close').focus(); // For quick closing with ENTER
    });

    $('#info-close').on('click', function() {
        // Remove info box
        $('#info-box-wrapper').css("display", "none");
    });

    // Activate listeners on all types
    $('#main-table').on('click', '.timestamp', function() {
        for (let i = 0; i < dumphisDataObj.size; i++) {
            if ($(this).attr('id') === `t_${i}`) { // On GUI selected timestamp
                newSelTimestamp = i;
                break;
            }
        }
    });

    $('#main-table').on('click', '.unit', function() {
        for (let i = 0; i < unitInfoDataObj.size; i++) {
            if ($(this).attr('id') === `u_${i}`) { // On GUI selected unit
                newSelUnit = i;
                break;
            }
        }
    });

    $('#main-table').on('click', '.cmnd', function() {
        for (let i = 0; i < n_createdCmnds; i++) {
            if ($(this).attr('id') === `c_${i}`) { // On GUI selected command
                newSelCmnd = i;
                break;
            }
        }
    });

    $('#main-table').on('click', '.step', function() {
        for (let i = 0; i < n_createdSteps; i++) {
            if ($(this).attr('id') === `s_${i}`) { // On GUI selected step
                newSelStep = i;
                break;
            }
        }
    });

    $('#btn-search').on('click', function() { 
        var $inputSearch = $('#input-search');

        $inputSearch.css("display", "block");

        setTimeout(function() {
            $inputSearch.addClass('animation-input-search');
        }, 10);
    });

    $('#btn-filter').on('click', function() { 
        var $filterBox = $('.filter-container');

        if ($filterBox.css("display") == "none") {
            $filterBox.css("display", "block");

            setTimeout(function() {
                $filterBox.addClass('animation-filter-box');
            }, 10);
        }
        else {
            $filterBox.css("display", "none");
            $filterBox.removeClass('animation-filter-box');
        }
    });

    $('#btn-filter-apply').on('click', function() { 
        var $filterBox = $('.filter-container');

        $filterBox.css("display", "none");
        $filterBox.removeClass('animation-filter-box');
    });

    let inputResults = [true, true, true];
    $('#filter-inp-unit').on('input', function() {
        let input = $(this).val();
        let result = Filter.checkInput(input);
        inputResults[0] = result;

        if (result) {
            $('#filter-inp-unit').removeClass('filter-inputs-wrong');
            if (Filter.allTrue(inputResults))
                $('#filter-error-load').addClass('animation-filter-error');
        } else {
            $('#filter-inp-unit').addClass('filter-inputs-wrong');
            $('#filter-error-container').css('display', 'block');
            $('#filter-error-load').removeClass('animation-filter-error');
        }
    });

    $('#filter-inp-cmnd').on('input', function() {
        let input = $(this).val();
        let result = Filter.checkInput(input);
        inputResults[1] = result;

        if (result) {
            $('#filter-inp-cmnd').removeClass('filter-inputs-wrong');
            if (Filter.allTrue(inputResults))
                $('#filter-error-load').addClass('animation-filter-error');
        } else {
            $('#filter-inp-cmnd').addClass('filter-inputs-wrong');
            $('#filter-error-container').css('display', 'block');
            $('#filter-error-load').removeClass('animation-filter-error');
        }
    });

    $('#filter-inp-step').on('input', function() {
        let input = $(this).val();
        let result = Filter.checkInput(input);
        inputResults[2] = result;

        if (result) {
            $('#filter-inp-step').removeClass('filter-inputs-wrong');
            if (Filter.allTrue(inputResults))
                $('#filter-error-load').addClass('animation-filter-error');
        } else {
            $('#filter-inp-step').addClass('filter-inputs-wrong');
            $('#filter-error-container').css('display', 'block');
            $('#filter-error-load').removeClass('animation-filter-error');
        }
    });

    $('#filter-error-load').on('animationend', function() {
        $('#filter-error-load').removeClass('animation-filter-error');
        $('#filter-error-container').css('display', 'none');
    })

    // Handle selections
    setInterval(function() {
        if ($("#filter-check-unit").is(':checked')) {
            $('#filter-inp-unit').removeAttr('disabled');
        } else {
            $('#filter-inp-unit').attr('disabled', true);
        }

        if ($("#filter-check-cmnd").is(':checked')) {
            $('#filter-inp-cmnd').removeAttr('disabled');
        } else {
            $('#filter-inp-cmnd').attr('disabled', true);
        }

        if ($("#filter-check-step").is(':checked')) {
            $('#filter-inp-step').removeAttr('disabled');
        } else {
            $('#filter-inp-step').attr('disabled', true);
        }

        if ($('.filter-container').css("display") == "none") {
            $('#filter-error-load').removeClass('animation-filter-error');
            $('#filter-error-container').css('display', 'none');
        }

        if (newSelTimestamp != selectedTimestamp) {
            selectedTimestamp = newSelTimestamp;
            //console.log(`New timestamp selected: ${selectedTimestamp}`);
            
            $('.timestamp-sel').removeClass('timestamp-sel');
            $(`#t_${selectedTimestamp}`).addClass('timestamp-sel');
            if ($("#check-scroll").is(':not(:checked)')) {
                jumpToSection(`#t_${selectedTimestamp}`);
            }

            // Select different unit
            newSelUnit = findNewSelUnit(selectedTimestamp);

            // TODO: auto select also cmnd and unit
        }

        if (newSelUnit != selectedUnit) {
            selectedUnit = newSelUnit;
            //console.log(`New unit selected: ${selectedUnit}`);

            $('.unit-sel').removeClass('unit-sel');
            $(`#u_${selectedUnit}`).addClass('unit-sel');

            $('#panel-unit').text(curUnitName);

            // Show commands
            table_addCmnds(selectedUnit);
            newSelCmnd = 0;
            selectedCmnd = REFRESH_VALUE; // To make sure command selection is refreshed
        }

        if (newSelCmnd != selectedCmnd) {
            selectedCmnd = newSelCmnd;
            //console.log(`New command selected: ${selectedCmnd}`);

            $('.cmnd-sel').removeClass('cmnd-sel');
            $(`#c_${selectedCmnd}`).addClass('cmnd-sel');

            // Show steps
            table_addSteps(selectedCmnd)
            newSelStep = 0;
            selectedStep = REFRESH_VALUE; // To make sure step selection is refreshed
        }

        if (newSelStep != selectedStep) {
            selectedStep = newSelStep;
            //console.log(`New step selected: ${selectedStep}`);

            $('.step-sel').removeClass('step-sel');
            $(`#s_${selectedStep}`).addClass('step-sel');
        }
    }, 1);
});



function readJSON(path) {
    return fetch(path)
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            return {
                data: data,
                size: data.length
            };
        });
}

function table_addTimestamps() {
    dumphisDataObj.data.forEach((item, i) => {
        let newRow = $(`<tr id="row_${i}">`);
        let cell = $(
            `<td id="t_${i}" class="timestamp unit-info-trig">
                ${item.time}
            </td>`);

        newRow.append(cell);
        $('#main-table tbody').append(newRow);
        maxTimestampIndex = i;
    });
}

function table_addUnits() {
    unitInfoDataObj.data.forEach((item, i) => {
        let name = (item.name === "") ? `??? (${item.id})` : `${item.name} (${item.id})`;
        let cell = $(
            `<td id="u_${i}" class="unit unit-info-trig">
                ${name}
            </td>`);
        $(`#row_${i}`).append(cell)
    });
}

function table_addCmnds(selUnit) {
    // Clean current displaying commands/steps
    $('.step').remove();
    $('.cmnd').remove();
    $('.placeholder').remove();

    let unitFuncId = unitInfoDataObj.data[selUnit].funcId;
    for (let i = 0; i < functionDataObj.size; i++) {
        let funcFuncId = functionDataObj.data[i].funcId;
        if (funcFuncId === unitFuncId) {
            curFuncIndex = i; // To be used for showing steps
            n_createdCmnds = showCmnds(functionDataObj.data[i].cmnds);
            break;
        } else {
            curFuncIndex = -1;
        }
    }
}

function table_addSteps(selCmnd) {
    // Clean current displaying steps
    if (n_createdCmnds > 0) {
        $('.step').remove();

        if (curFuncIndex != -1) {
            curCmndIndex = selCmnd;
            n_createdSteps = showSteps(functionDataObj.data[curFuncIndex].cmnds[selCmnd].steps);
        } else {
            console.warn("No function index found");
            curCmndIndex = -1;
        }
    }
}

function findNewSelUnit(selTimestamp) {
    let unitId = dumphisDataObj.data[selTimestamp].unitId;
    let result = -1;
    //console.log(`Unit id: ${unitId}`);

    unitInfoDataObj.data.forEach((item, i) => {
        if (item.id === unitId) {
            result = i;
            curUnitName = (item.name === "") ? `??? (${unitId})` : `${item.name} (${item.id})`;
        }
    });

    if (result < 0) {
        console.log("No unit is found!");
        return 0;
    }
    else {
        return result;
    }
    
}

function showCmnds(cmnds) {
    let n_cmnds = 0;
    cmnds.forEach((item, i) => {
        let name = (item.name === "") ? `??? (${item.id})` : `${item.name} (${item.id})`;
        let cell = $(`
            <td id="c_${i}" class="cmnd unit-info-trig">
                ${name}
            </td>`);
        $(`#row_${i}`).append(cell)
        n_cmnds++;
    });

    for (let i = 0; i < (N_EXTRA_CMNDS - n_cmnds); i++) {
        let cell = $('<td class="placeholder"></td>'); // Empty element to prevent steps going to wrong column
        $(`#row_${n_cmnds + i}`).append(cell)
    }

    return n_cmnds;
}

function showSteps(steps) {
    let n_steps = 0;
    steps.forEach((item, i) => {
        let name = (item.name === "") ? "???" : item.name;
        let cell = $(`
            <td id="s_${i}" class="step unit-info-trig">
                ${name}
            </td>`);
        $(`#row_${i}`).append(cell)
        n_steps++;
    });
    return n_steps;
}

function getFocusedColumn(elementId) {
    if (elementId === undefined) {
        console.error("No focused element found (undefined)!");
        curSelectedColumn = Columns.UNDEF;
        return false;
    }

    //console.log("Focused element id: " + elementId);
    if (elementId.startsWith('t')) {
        curSelectedColumn = Columns.TIMESTAMP;
    } else if (elementId.startsWith('u')) {
        curSelectedColumn = Columns.UNIT;
    } else if (elementId.startsWith('c')) {
        curSelectedColumn = Columns.COMMAND;
    } else if (elementId.startsWith('s')) {
        curSelectedColumn = Columns.STEP;
    } else {
        console.error("No focused element found!");
        curSelectedColumn = Columns.UNDEF;
        return false;
    }

    return true;
}

function createTimestampInfo(index) {
    let time = dumphisDataObj.data[index].time;
    let alarm = (dumphisDataObj.data[index].alarmId) == 0 ? "none" : dumphisDataObj.data[index].alarmId;
    let unit = dumphisDataObj.data[index].unitId;
    let cmnd = dumphisDataObj.data[index].cmdId;
    let step = dumphisDataObj.data[index].stepId;
    let state = dumphisDataObj.data[index].unitState;
    let stateColor = null;
    //console.log(`state: ${state}`);
    if (state == -1) 
        stateColor = ` class="state-error"`
    else if (state == 127) 
        stateColor = ` class="state-wait"`
    else
        stateColor = "";

    let unitName = `??? (${unit})`;
    unitInfoDataObj.data.forEach((item, i) => {
        if (item.id === unit) {
            unitName = (item.name === "") ? `??? (${unit})` : item.name;
        }
    });

    let text = `
        <div id="info-header">
            <h1>Timestamp info</h1>
        </div>
        <div id="info-content">
            <table class="info-list-table">
                <tr>
                    <td class="info-list-header">Time</td>
                    <td>${time}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Alarm</td>
                    <td>${alarm}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Changed unit</td>
                    <td>${unitName}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Changed command</td>
                    <td>${cmnd}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Changed step</td>
                    <td>${step}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Unit state</td>
                    <td${stateColor}>${state}</td>
                </tr>
            </table>
        </div>
    `;
    return text;
}

function createUnitInfo(index) {
    let name = unitInfoDataObj.data[index].name;
    let id = unitInfoDataObj.data[index].id;
    let nrCmnds = unitInfoDataObj.data[index].n_cmnds;
    let typeNr = unitInfoDataObj.data[index].type;
    let type = null;
    switch (typeNr) {
        case 0:
            type = "unit";
            break;

        case 1:
            type = "entity";
            break;

        case 2:
        default:
            type = "unknown";
            break;
    }

    let func = "unknown";
    let unitFuncId = unitInfoDataObj.data[index].funcId;
    for (let i = 0; i < functionDataObj.size; i++) {
        let funcFuncId = functionDataObj.data[i].funcId;
        if (funcFuncId === unitFuncId) {
            func = functionDataObj.data[i].name;
            break;
        }
    }

    let childrenTxt = "";
    for (let i = 0; i < MAX_NR_DATA_CON; i++) {
        let dc = unitInfoDataObj.data[index].dataConnections[i];
        let unitName = `??? (${dc})`;
        let txt = null;
        if (dc != 0) {
            unitInfoDataObj.data.forEach((item) => {
                if (item.id === dc) {
                    unitName = (item.name === "") ? `??? (${dc})` : `${item.name} (${dc})`;
                }
            });
            txt = `[${i}] ${unitName}`;
        } else {
            txt = `[${i}]`;
        }
        childrenTxt += `<span>${txt}</span><br>`;
    }

    let text = `
        <div id="info-header">
            <h1>Unit info</h1>
        </div>
        <div id="info-content">
            <table class="info-list-table">
                <tr>
                    <td class="info-list-header">Name</td>
                    <td>${name}</td>
                </tr>
                <tr>
                    <td class="info-list-header">ID</td>
                    <td>${id}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Nr. of (fall) commands</td>
                    <td>${nrCmnds}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Type</td>
                    <td>${type}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Function</td>
                    <td>${func}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Children</td>
                    <td>${childrenTxt}</td>
                </tr>
            </table>
        </div>
    `;
    return text;
}

function createCmndInfo(index) {
    if (curFuncIndex < 0) {
        console.error("Could not create command info box: func index < 0");
        return "";
    }    
    let cmnd = functionDataObj.data[curFuncIndex].cmnds[index];

    let stepSize = cmnd.steps.length;
    let childrenTxt = "";
    for (let i = 0; i < stepSize; i++) {
        let step = cmnd.steps[i];
        let stepName = `???`;
        if (step.name != "") {
            stepName = step.name;
        }
        let txt = null;
        txt = `${stepName}`;
        childrenTxt += `<span>${txt}</span><br>`;
    }

    let text = `
        <div id="info-header">
            <h1>Command info</h1>
        </div>
        <div id="info-content">
            <table class="info-list-table">
                <tr>
                    <td class="info-list-header">Name</td>
                    <td>${cmnd.name}</td>
                </tr>
                <tr>
                    <td class="info-list-header">ID</td>
                    <td>${cmnd.id}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Steps</td>
                    <td>${childrenTxt}</td>
                </tr>
            </table>
        </div>
    `;
    return text;
}

function createStepInfo(index) {
    if (curFuncIndex < 0 || curCmndIndex < 0) {
        console.error("Could not create command info box: func or cmnd index < 0");
        return "";
    }    
    let step = functionDataObj.data[curFuncIndex].cmnds[curCmndIndex].steps[index];

    let text = `
        <div id="info-header">
            <h1>Step info</h1>
        </div>
        <div id="info-content">
            <table class="info-list-table">
                <tr>
                    <td class="info-list-header">Name</td>
                    <td>${step.name}</td>
                </tr>
                <tr>
                    <td class="info-list-header">Link</td>
                    <td><a href="viewcode.html" target="_blank">view code</a></td>
                </tr>
            </table>
        </div>
    `;
    return text;
}

function createInfoContent(elementId) {
    let index = elementId.substring(2);

    switch (curSelectedColumn){
        case Columns.TIMESTAMP:
            return createTimestampInfo(index);

        case Columns.UNIT:
            return createUnitInfo(index);

        case Columns.COMMAND:
            return createCmndInfo(index);

        case Columns.STEP:
            return createStepInfo(index);

        default:
            let text = `
                <h1>Error:</h1>
                <p>No additional information available</p>
            `;
            return text;
    }
}

function animateScroll(top, duration) {
    $("#load-wrap").css("display", "flex");
    return new Promise((resolve) => {
        $('html, body').animate({ scrollTop: top }, duration, resolve);
    });
}

let newPos = 0;
let oldSectionIndex = 0;
async function jumpToSection(sectionId) {
    let headerHeight = $('#ctrl-panel').outerHeight() + $('#main-table-header').outerHeight();
    let targetPosition = $(sectionId).get(0).getBoundingClientRect().top;
    let offsetPosition = targetPosition - headerHeight;

    let sectionIndex = $(sectionId).attr('id').substring(2);
    if (sectionIndex < oldSectionIndex) {
        await animateScroll(0, 600);
        targetPosition = $(sectionId).get(0).getBoundingClientRect().top;
        offsetPosition = targetPosition - headerHeight;
        newPos = offsetPosition;
    }
    else {
        newPos += offsetPosition;
    }

    let outText = "Scroll: " + sectionId;
    outText += "\nHH: " + headerHeight +
               "\nTP: " + targetPosition +
               "\nOP: " + offsetPosition;
    console.log(outText);

    await animateScroll(newPos, 600);
    $("#load-wrap").css("display", "none");

    oldSectionIndex = sectionIndex
}