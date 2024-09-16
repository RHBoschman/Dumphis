export function checkInput(input) {
    console.log("Input is:" + input);
    if (input === '') {
        return true;
    } else {
        return false;
    }
}

export function allTrue(results) {
    for (let i = 0; i < results.length; i++) {
        if (results[i] != true) {
            return false;
        }
    }
    return true;
}