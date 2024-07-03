const N_EXTRA_CMNDS = 30; // Should NOT be bigger then number of dumphis items!!!

$(document).ready(function() {
    let unitInfoDataObj, functionDataObj;
    let n_createdCmnds = 0, n_createdSteps = 0;
    let curFuncIndex = -1;

    $('#but').on('click', function() {
        Promise.all([readDumphis(), readUnitInfo(), readFunctionData()])
            .then(([dumphisData, unitInfoData, functionData]) => {
                unitInfoDataObj = unitInfoData;
                functionDataObj = functionData;
                table_addTimestamps(dumphisData);
                table_addUnits(unitInfoData);
            })
            .catch(error => {
                console.error('Error fetching JSON:', error);
            });
        $(this).prop('disabled', true);
    });

    $('#main-table').on('click', '.unit', function() {
        for (let i = 0; i < unitInfoDataObj.size; i++) {
            if ($(this).attr('id') === `u_${i}`) { // Selected unit
                // Select unit
                $('.unit-sel').removeClass('unit-sel');
                $(this).addClass('unit-sel');

                // Clean current displaying commands/steps
                $('.step').remove();
                $('.cmnd').remove();
                $('.placeholder').remove();
                n_createdCmnds = 0; // In case funcId never matches

                // Show new commands
                let unitFuncId = unitInfoDataObj.data[i].funcId;
                for (let j = 0; j < functionDataObj.size; j++) {
                    let funcFuncId = functionDataObj.data[j].funcId;
                    if (funcFuncId === unitFuncId) {
                        curFuncIndex = j; // To be used for showing steps
                        n_createdCmnds = showCmnds(functionDataObj.data[j].cmnds);
                        break;
                    } else {
                        curFuncIndex = -1;
                    }
                }
                break;
            }
        }
    });

    $('#main-table').on('click', '.cmnd', function() {
        for (let i = 0; i < n_createdCmnds; i++) {
            if ($(this).attr('id') === `c_${i}`) { // Selected command
                // Select command
                $('.cmnd-sel').removeClass('cmnd-sel');
                $(this).addClass('cmnd-sel');

                // Remove current displaying steps
                $('.step').remove();

                if (curFuncIndex != -1) {
                    n_createdSteps = showSteps(functionDataObj.data[curFuncIndex].cmnds[i].steps);
                } else {
                    let cell = $(`<td>No function index found</td>`);
                    $(`#row_0`).append(cell); // I have to look at this! When is this removed?
                }
            }
        }
    });

    $('#main-table').on('click', '.step', function() {
        for (let i = 0; i < n_createdSteps; i++) {
            if ($(this).attr('id') === `s_${i}`) { // Selected step
                // Select step
                $('.step-sel').removeClass('step-sel');
                $(this).addClass('step-sel');
            }
        }
    });
});

function table_addTimestamps(data) {
    data.forEach((item, i) => {
        let newRow = $(`<tr id="row_${i}">`);
        let cell = $(`<td class="timestamp">${item.time}</td>`);

        newRow.append(cell);
        $('#main-table tbody').append(newRow);
    });
}

function table_addUnits(units) {
    units.data.forEach((item, i) => {
        let cell = $(`<td id="u_${i}" class="unit">${item.name}</td>`);
        $(`#row_${i}`).append(cell)
    });
}

function readDumphis() {
    return fetch('../exports/DumphisData.json')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            return data;
        });
}

function readUnitInfo() {
    return fetch('../exports/UnitInfoData.json')
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

function readFunctionData() {
    return fetch('../exports/FunctionData.json')
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

function showCmnds(cmnds) {
    let n_cmnds = 0;
    cmnds.forEach((item, i) => {
        let cell = $(`<td id="c_${i}" class="cmnd">${item.name}</td>`);
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
        let cell = $(`<td id="s_${i}" class="step">${item.name}</td>`);
        $(`#row_${i}`).append(cell)
        n_steps++;
    });
    return n_steps;
}

function removeElementById(id) {
    let element = $('#' + id);
    if (element.length > 0) {
        element.remove();
    } else {
        console.log('Element with ID "' + id + '" not found.');
    }
}