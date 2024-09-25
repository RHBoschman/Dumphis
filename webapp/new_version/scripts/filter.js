export class Filter {
    // Constr:
    constructor() {}

    Types = Object.freeze({
        UNIT: 0,
        COMMAND: 1,
        STEP: 2
    });

    allFilters = {
        units: [],
        cmnds: [],
        steps: []
    };

    // Check the filter inputs
    checkInput(input) {
        if (input === '') {
            return { isValid: true, indexes: [] };
        } else {
            const regex = /^[0-9\s,]*$/;
            if (regex.test(input)) {

                const subStrs = input.split(',');
                const subStrsTrimmed = subStrs.map(subStrs => subStrs.trim());
                const resultIndexes = subStrsTrimmed.filter(substring => substring !== '');
                let spaceInRes = resultIndexes.some(subStr => /\s/.test(subStr));
                const indexes = resultIndexes.map(subStr => parseInt(subStr, 10))
                                        .filter(num => !isNaN(num));

                return {
                    isValid: !spaceInRes,
                    indexes: indexes
                };
                
            } else
                return { isValid: false, indexes: [] };
        }
    }

    setUnitFilters(indexes) {
        this.allFilters.units = indexes;
    }

    // setCmndFilters(indexes) {
    //     this.allFilters.cmnds = indexes;
    // }

    // setStepFilters(indexes) {
    //     this.allFilters.steps = indexes;
    // }

    // allTrue(results) {
    //     for (let i = 0; i < results.length; i++) {
    //         if (results[i] != true) {
    //             return false;
    //         }
    //     }
    //     return true;
    // }

    // Check if index of type is in filter
    // true = in filter or there are no filters -> so render
    // false = not in filter and filters are not empty -> do not render
    checkFor(type, index) {
        if (this.allFilters.units.length == 0 &&
            this.allFilters.cmnds.length == 0 &&
            this.allFilters.steps.length == 0
        ) { // There are no filters
            return true;
        } 
        else { // There are filters
            switch(type) {
                case this.Types.UNIT:
                    return this.allFilters.units.includes(index);
                case this.Types.COMMAND:
                    return this.allFilters.cmnds.includes(index);
                case this.Types.STEP:
                    return this.allFilters.steps.includes(index);
                default:
                    console.error(`Type ${type} does not exist`);
                    return false;
            }
        }
    }
}

