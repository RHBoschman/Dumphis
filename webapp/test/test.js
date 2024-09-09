$(document).ready(function () {

    $(window).on('scroll', function() {
        let scrollTop = $(window).scrollTop();
        $('#test').text(scrollTop);
    });

});
