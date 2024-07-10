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


const dumphisPath = '../exports/DumphisData.json';
const unitInfoPath = '../exports/UnitInfoData.json';

$(document).ready(function() {
    let selectedTimestamp = 0;
    let selectedUnit = 0;

    let dumphisDataObj = null;
    let unitInfoDataObj = null;

    // Initialize app on button click
    $('#but').on('click', function() {
        Promise.all([readJSON(dumphisPath), readJSON(unitInfoPath)])
            .then(([dumphisData, unitInfoData]) => {
                dumphisDataObj = dumphisData;
                unitInfoDataObj = unitInfoData;
                table_addTimestamps(dumphisData);
                table_addUnits(unitInfoData);
            })
            .then(() => {
                // Select all first items
                if ($('#t_0').length > 0) {
                    $('.timestamp-sel').removeClass('timestamp-sel');
                    $('#t_0').addClass('timestamp-sel');
                }

                if ($('#u_0').length > 0) {
                    $('.unit-sel').removeClass('unit-sel');
                    $('#u_0').addClass('unit-sel');
                }
            })
            .catch(error => {
                console.error('Error fetching JSON:', error);
            });
        $(this).prop('disabled', true);
    });

    // Activate listeners on all types
    $('#main-table').on('click', '.timestamp', function() {
        for (let i = 0; i < dumphisDataObj.size; i++) {
            if ($(this).attr('id') === `t_${i}`) { // On GUI selected timestamp
                // Select timestamp
                $('.timestamp-sel').removeClass('timestamp-sel');
                $(this).addClass('timestamp-sel');
                selectedTimestamp = i;
            }
        }
    });

    $('#main-table').on('click', '.unit', function() {
        for (let i = 0; i < unitInfoDataObj.size; i++) {
            if ($(this).attr('id') === `u_${i}`) { // On GUI selected unit
                // Select unit
                $('.unit-sel').removeClass('unit-sel');
                $(this).addClass('unit-sel');
                selectedUnit = i;
            }
        }
    });

    // $('#main-table').on('click', '.cmnd', function() {

    // });

    // $('#main-table').on('click', '.step', function() {

    // });
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

function table_addTimestamps(timestamps) {
    timestamps.data.forEach((item, i) => {
        let newRow = $(`<tr id="row_${i}">`);
        let cell = $(`<td id="t_${i}" class="timestamp">${item.time}</td>`);

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