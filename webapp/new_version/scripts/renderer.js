import { Filter } from "./filter.js";

export class Renderer {
    constructor() {
        this.filter = new Filter();

        this.dumphisPath = '../../exports/DumphisData.json';
        this.unitInfoPath = '../../exports/UnitInfoData.json';
        this.funcDataPath = '../../exports/FunctionData.json';
        this.dumphisDataObj = null;
        this.unitInfoDataObj = null;
        this.functionDataObj = null;
    }

    init () {
        // Collect JSON data
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

    // Function to read data from the generated JSON files
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

    renderData() {
        return new Promise((resolve, reject) => {
            console.log("Starting to render all data");

            console.log("Applying filters...");

            let text = "";

            try {
            this.dumphisDataObj.data.forEach(item => {
                let rowId = item.ID;
                let time = item.time;
                let unit = item.unitId;
                let cmnd = item.cmdId;
                let step = item.stepId;

                text += 
                    `<tr class="data-sample" id="${rowId}">
                        <td class="col-general col-time" >${time}</td>
                        <td class="col-general col-unit">${unit}</td>
                        <td class="col-general col-cmnd">${cmnd}</td>
                        <td class="col-general col-step">${step}</td>
                    </tr>`;
            });

            resolve(text);

            } catch(error) {
                console.error("Error with rendering: " + error);
                let errorText = `<p id="init-error">! ERROR while rendering data. See console for any catched errors</p>`;
                reject(errorText);
            }
        });
    }
}