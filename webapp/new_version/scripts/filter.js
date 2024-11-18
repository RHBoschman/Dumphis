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
        if ($('#filter-check-unit').prop('checked'))
            this.allFilters.units = indexes;
    }

    setCmndFilters(indexes) {
        if ($('#filter-check-cmnd').prop('checked'))
            this.allFilters.cmnds = indexes;
    }

    setStepFilters(indexes) {
        if ($('#filter-check-step').prop('checked'))
            this.allFilters.steps = indexes;
    }

    resetAll() {
        this.allFilters.units = [];
        this.allFilters.cmnds = [];
        this.allFilters.steps = [];
    }

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
            let isEmpty;

            switch(type) {
                case this.Types.UNIT:
                    isEmpty = (this.allFilters.units.length == 0);
                    return (isEmpty ? true : this.allFilters.units.includes(index));

                case this.Types.COMMAND:
                    isEmpty = (this.allFilters.cmnds.length == 0);
                    return (isEmpty ? true : this.allFilters.cmnds.includes(index));

                case this.Types.STEP:
                    isEmpty = (this.allFilters.steps.length == 0);
                    return (isEmpty ? true : this.allFilters.steps.includes(index));

                default:
                    console.error(`Type ${type} does not exist`);
                    return false;
            }
        }
    }

    checkContinuously() {
        let unit_check = this.checkInput($('#filter-inp-unit').val());
        let cmnd_check = this.checkInput($('#filter-inp-cmnd').val());
        let step_check = this.checkInput($('#filter-inp-step').val());

        if (!unit_check.isValid || !cmnd_check.isValid || !step_check.isValid) {
            // Error
            $('#filter-error-container').css('display', 'block');
            $('#filter-error-load').removeClass('animation-filter-error');
        }
        else {
            // No error
            $('#filter-error-load').addClass('animation-filter-error');
        }

        // Specific input bar
        if (!unit_check.isValid) 
            $('#filter-inp-unit').addClass('filter-inputs-wrong');
        else 
            $('#filter-inp-unit').removeClass('filter-inputs-wrong');   

        if (!cmnd_check.isValid) 
            $('#filter-inp-cmnd').addClass('filter-inputs-wrong');
        else
            $('#filter-inp-cmnd').removeClass('filter-inputs-wrong'); 

        if (!step_check.isValid) 
            $('#filter-inp-step').addClass('filter-inputs-wrong');
        else
            $('#filter-inp-step').removeClass('filter-inputs-wrong'); 
    }

}

