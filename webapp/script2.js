// This file is used to develop the same functionality as script.js
// but from a different approach. Namely: using standardized functions 
// to show the data columns using some variables like 'xxx_selected'.
// selectedTimeStamp
// selectedUnit
// selectedCommand
// selectedStep

// Procedure:
// - add listener to button click
// - add timestamps and units
// - give every column a 'selected' variable
// - Create some kind of loop that constantly checks the
//   select variables on a change. Then update accordingly.
//   loop is not possible (would block all listeners), but
//   you can do something like interval (chat gpt)

const N_EXTRA_CMNDS = 30; // Should NOT be bigger then number of dumphis items!!!
const REFRESH_VALUE = -1;

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

    // General handling of info button
    $(document).on('click', '.unit-info-button', function() {
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

    // Handle selections
    setInterval(function() {
        if (newSelTimestamp != selectedTimestamp) {
            selectedTimestamp = newSelTimestamp;
            console.log(`New timestamp selected: ${selectedTimestamp}`);
            
            $('.timestamp-sel').removeClass('timestamp-sel');
            $(`#t_${selectedTimestamp}`).addClass('timestamp-sel');

            // Select different unit
            newSelUnit = findNewSelUnit(selectedTimestamp);
            // TODO: auto select also cmnd and unit
        }

        if (newSelUnit != selectedUnit) {
            selectedUnit = newSelUnit;
            console.log(`New unit selected: ${selectedUnit}`);

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
            console.log(`New command selected: ${selectedCmnd}`);

            $('.cmnd-sel').removeClass('cmnd-sel');
            $(`#c_${selectedCmnd}`).addClass('cmnd-sel');

            // Show steps
            table_addSteps(selectedCmnd)
            newSelStep = 0;
            selectedStep = REFRESH_VALUE; // To make sure step selection is refreshed
        }

        if (newSelStep != selectedStep) {
            selectedStep = newSelStep;
            console.log(`New step selected: ${selectedStep}`);

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
            `<td id="t_${i}" class="timestamp">
                ${item.time}
                <button class="unit-info-button">i</button>
            </td>`);

        newRow.append(cell);
        $('#main-table tbody').append(newRow);
        maxTimestampIndex = i;
    });
}

function table_addUnits() {
    unitInfoDataObj.data.forEach((item, i) => {
        let name = (item.name === "") ? `??? (${item.id})` : item.name;
        let cell = $(
            `<td id="u_${i}" class="unit">
                ${name}
                <button class="unit-info-button">i</button>
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
            n_createdSteps = showSteps(functionDataObj.data[curFuncIndex].cmnds[selCmnd].steps);
        } else {
            console.warn("No function index found");
        }
    }
}

function findNewSelUnit(selTimestamp) {
    let unitId = dumphisDataObj.data[selTimestamp].unitId;
    let result = -1;
    console.log(`Unit id: ${unitId}`);

    unitInfoDataObj.data.forEach((item, i) => {
        if (item.id === unitId) {
            result = i;
            curUnitName = (item.name === "") ? `??? (${unitId})` : item.name;
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
        let name = (item.name === "") ? "???" : item.name;
        let cell = $(`
            <td id="c_${i}" class="cmnd">
                ${name}
                <button class="unit-info-button">i</button>
            </td>`);
        $(`#row_${i}`).append(cell)
        n_cmnds++;
    });

    for (let i = 0; i < (N_EXTRA_CMNDS - n_cmnds); i++) {
        let cell = $('<td class="placeholder"></td>'); // Empty element
        $(`#row_${n_cmnds + i}`).append(cell)
    }

    return n_cmnds;
}

function showSteps(steps) {
    let n_steps = 0;
    steps.forEach((item, i) => {
        let name = (item.name === "") ? "???" : item.name;
        let cell = $(`
            <td id="s_${i}" class="step">
                ${name}
                <button class="unit-info-button">i</button>
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

    console.log("Focused element id: " + elementId);
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
    let text = `
        <div id="info-header">
            <h1>Timestamp info</h1>
        </div>
        <div id="info-content">
            <table class="info-list-table">
                <tr>
                    <td class="info-list-header">Time</td>
                    <td>2024-03-28  13:04:54</td>
                </tr>
                <tr>
                    <td class="info-list-header">Alarm</td>
                    <td>none</td>
                </tr>
                <tr>
                    <td class="info-list-header">Changed unit</td>
                    <td>1</td>
                </tr>
                <tr>
                    <td class="info-list-header">Changed command</td>
                    <td>1</td>
                </tr>
                <tr>
                    <td class="info-list-header">Changed step</td>
                    <td>1</td>
                </tr>
                <tr>
                    <td class="info-list-header">Unit state</td>
                    <td>255</td>
                </tr>
            </table>
        </div>
    `;
    return text;
}

function createUnitInfo(index) {
    let text = `
        <div id="info-header">
            <h1>Unit info</h1>
        </div>
        <div id="info-content">
            <table class="info-list-table">
                <tr>
                    <td class="info-list-header">Name</td>
                    <td>U_SEQUENCE</td>
                </tr>
                <tr>
                    <td class="info-list-header">ID</td>
                    <td>362</td>
                </tr>
                <tr>
                    <td class="info-list-header">Nr. of (fall) commands</td>
                    <td>4</td>
                </tr>
                <tr>
                    <td class="info-list-header">Type</td>
                    <td>Entity</td>
                </tr>
                <tr>
                    <td class="info-list-header">Function</td>
                    <td>sequence</td>
                </tr>
                <tr>
                    <td class="info-list-header">Children</td>
                    <td>
                        <span>1</span><br>
                        <span>2</span><br>
                        <span>3</span><br>
                        <span>4</span><br>
                        <span>5</span><br>
                        <span>6</span><br>
                        <span>1</span><br>
                        <span>2</span><br>
                        <span>3</span><br>
                        <span>4</span><br>
                        <span>5</span><br>
                        <span>6</span><br>
                    </td>
                </tr>
            </table>
        </div>
    `;
    return text;
}

function createCmndInfo(index) {
    let text = `
        <div id="info-header">
            <h1>Command info</h1>
        </div>
    `;
    return text;
}

function createStepInfo(index) {
    let text = `
        <div id="info-header">
            <h1>Step info</h1>
        </div>
    `;
    return text;
}

function createInfoContent(elementId) {
    let index = elementId.substring(2, 2)

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