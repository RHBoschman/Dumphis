export class Filter {
    constructor() {}

    allFilters = {
        units: [],
        cmnds: [],
        steps: []
    };

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

    setCmndFilters(indexes) {
        this.allFilters.cmnds = indexes;
    }

    setStepFilters(indexes) {
        this.allFilters.steps = indexes;
    }

    getFilters() {
        return this.allFilters;
    }

    allTrue(results) {
        for (let i = 0; i < results.length; i++) {
            if (results[i] != true) {
                return false;
            }
        }
        return true;
    }
}

