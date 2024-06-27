
$(document).ready(function() {
    $('#but').on('click', addRows);
})

function addRows() {
    for (let i = 0; i < 5; i++) {
        let newRow = $('<tr>');
        let cols = '';

        cols += '<td>T</td>';
        cols += '<td>U</td>';
        cols += '<td>C</td>';
        cols += '<td>S</td>';

        newRow.append(cols);
        $('#main-table').append(newRow);
    }
}