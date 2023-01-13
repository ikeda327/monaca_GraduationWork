$(function () {
    // メニュータブ
    $('.tab_panel').css('display', 'none');
    var hash = location.hash;
    if (hash.match(/#tab01/)) {
        $('.tab_panel').eq(0).fadeIn();
        $('#tab_menu li').eq(0).addClass('selected');
    } else if (hash.match(/#tab02/)) {
        $('.tab_panel').eq(1).fadeIn();
        $('#tab_menu li').eq(1).addClass('selected');
    } else if (hash.match(/#tab03/)) {
        $('.tab_panel').eq(2).fadeIn();
        $('#tab_menu li').eq(2).addClass('selected');
    } else {
        $('.tab_panel').eq(0).fadeIn();
        $('#tab_menu li').eq(0).addClass('selected');
    }
    $('#tab_menu li a').click(function () {
        var thisUrl = $(this).attr('href');
        history.replaceState('', '', thisUrl);
        var index01 = $('#tab_menu li a').index(this);
        $('.tab_panel').css('display', 'none');
        $('.tab_panel').eq(index01).fadeIn();
        $('#tab_menu li').removeClass('selected');
        $(this).parent().addClass('selected');
    });

    // ペイントページ
    // ツールボタン
    $('.paint-icon').click(function () {
        $('.paint-icon').removeClass('selected2');
        $(this).addClass('selected2');
    });

    // ポップアップ
    $(".open").on('click', function () {
        $(".popup").addClass('view');
    });
    $(".close").on('click', function () {
        $(".popup").removeClass('view');
    });

})