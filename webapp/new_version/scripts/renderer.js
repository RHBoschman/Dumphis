import { Filter } from "./filter.js";

export class Renderer {
    // Constr: initialize variables
    constructor() {
        this.filter = new Filter();

        this.dumphisPath = '../../exports/DumphisData.json';
        this.unitInfoPath = '../../exports/UnitInfoData.json';
        this.funcDataPath = '../../exports/FunctionData.json';
        this.dumphisDataObj = null;
        this.unitInfoDataObj = null;
        this.functionDataObj = null;
        this.lastRenderId = 0;
    }

    // Collect all JSON data
    init () {
        return Promise.all([
            this.readJSON(this.dumphisPath), 
            this.readJSON(this.unitInfoPath), 
            this.readJSON(this.funcDataPath)])
            .then(([dumphisData, unitInfoData, funcData]) => {
                this.dumphisDataObj = dumphisData;
                this.unitInfoDataObj = unitInfoData;
                this.functionDataObj = funcData;
                console.info('Reading JSON files done');
            })
            .catch(error => {
                console.error('Error fetching JSON:', error);
                throw error;
            });
    }

    // Read data from the generated JSON files
    readJSON(path) {
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

    // Render the data (filters applied)
    renderData() {
        return new Promise((resolve, reject) => {
            console.log("Starting to render all data");
            console.log("Applying filters...");

            let unit_filters = this.filter.checkInput($('#filter-inp-unit').val());
            let cmnd_filters = this.filter.checkInput($('#filter-inp-cmnd').val());
            let step_filters = this.filter.checkInput($('#filter-inp-step').val());

            let text = "";
            let totalCounter = 0;
            let filteredCounter = 0;
            try {
                if (unit_filters.isValid && cmnd_filters.isValid && step_filters.isValid) {
                    this.filter.resetAll();
                    this.filter.setUnitFilters(unit_filters.indexes);
                    this.filter.setCmndFilters(cmnd_filters.indexes);
                    this.filter.setStepFilters(step_filters.indexes);
                    
                    this.dumphisDataObj.data.forEach(item => {
                        let rowId = item.ID;
                        let time = item.time;
                        let unitId = item.unitId;
                        let cmndId = item.cmdId;
                        let stepId = item.stepId;
                        let alarmId = item.alarmId;
                        let stateId = item.unitState;
                        if (this.filter.checkFor(this.filter.Types.UNIT, unitId)    && 
                            this.filter.checkFor(this.filter.Types.COMMAND, cmndId) &&
                            this.filter.checkFor(this.filter.Types.STEP, stepId) 
                        ) {
                            let unitObj = this.findUnit(unitId);
                            let unitName = this.findUnitName(unitObj);
                            let funcObj = this.findFunction(unitObj.funcId);
                            let cmndObj = this.findCmnd(funcObj, cmndId);
                            let cmndName = this.findCmndName(cmndObj, cmndId);
                            let stepName = (stepId == 0) ? "START_STEP" : stepId;
                            let addStatusBox = this.getStatusBox(stateId, alarmId);
                            text += 
                                `<tr class="data-sample r${filteredCounter}" id="${rowId}">
                                    <td class="col-general col-time">${time}${addStatusBox}</td>
                                    <td class="col-general col-unit">${unitName}</td>
                                    <td class="col-general col-cmnd">${cmndName}</td>
                                    <td class="col-general col-step">${stepName}</td>
                                </tr>`;
                            this.lastRenderId = filteredCounter;
                            filteredCounter++;
                        }
                        totalCounter++;
                    });

                    console.log(`Created text for ${filteredCounter} / ${totalCounter} items`);
                    $('.data-sample').remove();
                    resolve(text);
                }
                else {
                    console.warn("Filter inputs are not correct. No data rendered");
                    resolve($('#main-table').text());
                }
            } catch(error) {
                console.error("Error with rendering: " + error);
                let errorText = `<p id="init-error">! ERROR while rendering data. See console for any catched errors</p>`;
                $('.data-sample').remove();
                reject(errorText);
            }
        });
    }

    // Find unit object by dumphis unit id
    findUnit(unitId) {
        return this.unitInfoDataObj.data.find(item => item.id === unitId);
    }

    // Find name of unit from unit JSON object
    findUnitName(foundUnit) {
        return (foundUnit.name.length == 0) ? `??? (${foundUnit.id})` : `${foundUnit.name} (${foundUnit.id})`
    }

    // Find function object from function id (from unit object)
    findFunction(funcId) {
        return this.functionDataObj.data.find(item => item.funcId === funcId);
    }

    // Find command object from function object and dumphis command id
    findCmnd(funcObj, cmndId) {
        for (let i = 0; i < funcObj.cmnds.length; i++) {
            if (funcObj.cmnds[i].id === cmndId) {
                return {
                    obj: funcObj.cmnds[i],
                    isFall: false
                };
            }
        }

        for (let i = 0; i < funcObj.fallCmnds.length; i++) {
            if (funcObj.fallCmnds[i].id === cmndId) {
                return {
                    obj: funcObj.fallCmnds[i],
                    isFall: true
                };
            }
        }

        return {
            obj: null,
            isFall: false
        }
    }

    // Find command name from command object
    findCmndName(cmndObj, cmndId) {
        if (cmndObj.obj != null) {
            if (cmndObj.isFall)
                return `(FALL) ${cmndObj.obj.name} (${cmndObj.obj.id})`;
            else
                return `${cmndObj.obj.name} (${cmndObj.obj.id})`;
        } else
            return `error (${cmndId})`;
    }

    // Generate HTML for a status box
    getStatusBox(status, text) {
        let product;
        switch (status) {
            case -1:
                product = `<div class="status-box" id="status-box-alarm">${text}</div>`;
                break;

            case 127: // Ignore 'text' since function call always passes alarmId
                product = `<div class="status-box" id="status-box-wait"></div>`;
                break;

            default:
                product = "";
                break;
        }

        return product;
    }

    // Show filter error
    showFilterError(result) {
        if (result) {
            // if (filter.allTrue(inputResults))
            //     $('#filter-error-load').addClass('animation-filter-error');
        } else {
            $('#filter-inp-unit').addClass('filter-inputs-wrong');
            $('#filter-error-container').css('display', 'block');
            $('#filter-error-load').removeClass('animation-filter-error');
        }
    }

    // Update user continuously on filters
    checkFiltersContinuously() {
        this.filter.checkContinuously();
    }
}