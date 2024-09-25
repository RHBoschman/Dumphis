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
                let unit = item.unitId;
                let cmnd = item.cmdId;
                let step = item.stepId;

                if (this.filter.checkFor(this.filter.Types.UNIT, unit)    || 
                    this.filter.checkFor(this.filter.Types.COMMAND, cmnd) ||
                    this.filter.checkFor(this.filter.Types.STEP, step) 
                ) {
                    text += 
                        `<tr class="data-sample" id="${rowId}">
                            <td class="col-general col-time" >${time}</td>
                            <td class="col-general col-unit">${unit}</td>
                            <td class="col-general col-cmnd">${cmnd}</td>
                            <td class="col-general col-step">${step}</td>
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
}