import { Filter } from "./filter.js";

export class Renderer {
    // Constr: initialize variables
    constructor() {
        this.filter = new Filter();
        
        // TMP: lines below is just for testing the filters 
        // let result = this.filter.checkInput("430");
        // this.filter.setUnitFilters(result.indexes);

        this.dumphisPath = '../../exports/DumphisData.json';
        this.unitInfoPath = '../../exports/UnitInfoData.json';
        this.funcDataPath = '../../exports/FunctionData.json';
        this.dumphisDataObj = null;
        this.unitInfoDataObj = null;
        this.functionDataObj = null;
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

            let text = "";
            let totalCounter = 0;
            let filteredCounter = 0;

            try {
            this.dumphisDataObj.data.forEach(item => {
                let rowId = item.ID;
                let time = item.time;
                let unitId = item.unitId;
                let cmndId = item.cmdId;
                let stepId = item.stepId;
                let alarmId = item.alarmId;
                let stateId = item.unitState;

                if (this.filter.checkFor(this.filter.Types.UNIT, unitId)    || 
                    this.filter.checkFor(this.filter.Types.COMMAND, cmndId) ||
                    this.filter.checkFor(this.filter.Types.STEP, stepId) 
                ) {
                    let unitObj = this.findUnit(unitId);
                    let unitName = this.findUnitName(unitObj);
                    let funcObj = this.findFunction(unitObj.funcId);
                    let cmndObj = this.findCmnd(funcObj, cmndId);
                    let cmndName = this.findCmndName(cmndObj, cmndId);
                    let stepName = (stepId == 0) ? "START_STEP" : stepId;

                    let addColor = "";
                    let addAlarmUnit = "";
                    if (stateId < 0) {
                        addColor = 'id="col-alarm"';
                        addAlarmUnit = ` \u2192${alarmId}`;
                    }
                    else if (stateId == 127)
                        addColor = 'id="col-wait"';

                    text += 
                        `<tr class="data-sample" id="${rowId}">
                            <td ${addColor} class="col-general col-time">${time+addAlarmUnit}</td>
                            <td ${addColor} class="col-general col-unit">${unitName}</td>
                            <td ${addColor} class="col-general col-cmnd">${cmndName}</td>
                            <td ${addColor} class="col-general col-step">${stepName}</td>
                        </tr>`;

                    filteredCounter++;
                }
                totalCounter++;
            });

            console.log(`Created text for ${filteredCounter} / ${totalCounter} items`);
            resolve(text);

            } catch(error) {
                console.error("Error with rendering: " + error);
                let errorText = `<p id="init-error">! ERROR while rendering data. See console for any catched errors</p>`;
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
}