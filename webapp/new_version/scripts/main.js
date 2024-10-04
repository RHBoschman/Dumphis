import { Renderer } from "./renderer.js";
import { Searcher } from "./searcher.js";

let selectedRow = 0;

$(document).ready(function() {
    const renderer = new Renderer();

    $('#load-wrap').css("display", "flex");
    renderer.init()
    .then(() => {
        console.log("Initialization successful");
        $('#load-wrap').css("display", "none");


/********* ALL EVENT LISTENERS (START) *********/

        // Start button to read and render data
        $('#render').on('click', () => {
            generalRenderData(renderer);
            enablePanel();
            selectRow();
        });

        // Arrow buttons iterate over the time stamps
        $('#next').on('click', function() {
            if (selectedRow < renderer.lastRenderId)
                selectedRow++;
            selectRow();
        });

        $('#previous').on('click', function() {
            if (selectedRow > 0)
                selectedRow--;
            selectRow();
        });

        // Trigger for info boxes
        $(document).on('dblclick', '.info-trig', function() {});

        // Removal of info box
        $('#info-close').on('click', function() {
            $('#info-box-wrapper').css("display", "none");
        });

        // Trigger for search bar
        $('#btn-search').on('click', function() { 
            var $inputSearch = $('#input-search');

            $inputSearch.css("display", "block");

            setTimeout(function() {
                $inputSearch.addClass('animation-input-search');
            }, 10);
        });

        // Events to configure filters
        $('#btn-filter').on('click', function() { 
            var $filterBox = $('.filter-container');

            if ($filterBox.css("display") == "none") {
                $filterBox.css("display", "block");

                setTimeout(function() {
                    $filterBox.addClass('animation-filter-box');
                }, 10);
            }
            else {
                $filterBox.css("display", "none");
                $filterBox.removeClass('animation-filter-box');
            }
        });

        // Filter apply is same as render
        $('#btn-filter-apply').on('click', () => {
            generalRenderData(renderer);
            selectedRow = 0; // Select first row again
            selectRow();
        });

        $('#btn-filter-reset').on('click', function() { 
            let filterInputs = [$('#filter-inp-unit'), $('#filter-inp-cmnd'), $('#filter-inp-step')];

            filterInputs.forEach(input => {
                input.val('');
            });
        });

        // Handle filter inputs
        $('#filter-inp-unit').on('input', function() {});
        $('#filter-inp-cmnd').on('input', function() {});
        $('#filter-inp-step').on('input', function() {});

        $('#filter-error-load').on('animationend', function() {
            $('#filter-error-load').removeClass('animation-filter-error');
            $('#filter-error-container').css('display', 'none');
        });

/********* ALL EVENT LISTENERS (END) *********/

        // Main loop
        setInterval(function() {
            if ($("#filter-check-unit").is(':checked')) {
                $('#filter-inp-unit').removeAttr('disabled');
            } else {
                $('#filter-inp-unit').attr('disabled', true);
            }

            if ($("#filter-check-cmnd").is(':checked')) {
                $('#filter-inp-cmnd').removeAttr('disabled');
            } else {
                $('#filter-inp-cmnd').attr('disabled', true);
            }

            if ($("#filter-check-step").is(':checked')) {
                $('#filter-inp-step').removeAttr('disabled');
            } else {
                $('#filter-inp-step').attr('disabled', true);
            }

            if ($('.filter-container').css("display") == "none") {
                $('#filter-error-load').removeClass('animation-filter-error');
                $('#filter-error-container').css('display', 'none');
            }
        }, 1);
    })
    .catch(error => {
        console.error("Initialization failed:", error);
        $('body').append('<p id="init-error">! ERROR while retrieving data. See console for any catched errors</p>');
        $('#load-wrap').css("display", "none");
    });
});

function generalRenderData(renderer) {
    $('#load-wrap').css("display", "flex");
    $('.data-sample').remove();
    renderer.renderData().then(text => {
        $('#main-table').append(text);
    }).catch(text => {
        $('#main-table').append(text);
    });
    $('#load-wrap').css("display", "none");
}

function enablePanel() {
    $('.arrows').prop('disabled', false);
    $('.panel-text').css('display', 'block');
    $('#btn-search').prop('disabled', false);
    $('#btn-filter').prop('disabled', false);
    $('#btn-help').prop('disabled', false);
    $('check-scroll').prop('disabled', false);
}

function selectRow() {
    $('.selected').removeClass('selected');
    $(`.r${selectedRow}`).addClass('selected');
    console.log("selected first row: " + selectedRow);
}