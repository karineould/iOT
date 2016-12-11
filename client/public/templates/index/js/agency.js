// Agency Theme JavaScript

(function($) {
    "use strict"; // Start of use strict

    // Highlight the top nav as scrolling occurs
    $('body').scrollspy({
        target: '.navbar-fixed-top',
        offset: 51
    });

    // Closes the Responsive Menu on Menu Item Click
    $('.navbar-collapse ul li a').click(function(){ 
            $('.navbar-toggle:visible').click();
    });

    // Offset for Main Navigation
    $('#mainNav').affix({
        offset: {
            top: 100
        }
    });

    $('#btnConnect').click(function(){
        $('#modalConnect').modal('show');
    });


    $('#connectToRasp').on('click', function() {
        var ip = $('#inputIP').val();
        var port = $('#inputPort').val();

        window.location.href = '/data?ip=' + ip + '&port=' + port;
    });

})(jQuery); // End of use strict
