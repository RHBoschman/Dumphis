$(document).ready(function () {
    $('#but').on('click', function() {
        addPara();
    })
});

function addPara() {
    $("#a-div").append('<p id="pizza">New</p>');
}