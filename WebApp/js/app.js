    var images = ['url(./images/backImg1.png)', 'url(./images/backImg2.png)', 'url(./images/backImg3.png)', 'url(./images/backImg4.png)']; // ランダム表示させたい画像のパス
    $(function () {
        // 背景画像

        $(function () {
            var backgroundRandom = function () {
                var number = Math.floor(Math.random() * images.length); // 0~3の数値を算出 
                var selectedImg = images[number]; // 算出された数値を元に、1行目の配列から取り出す
                $('.body-wrapper').css('background-image', selectedImg); // cssにランダムに選ばれた画像を背景設定する
            };
            setInterval(backgroundRandom, 3000); // 1000msごとにランダム切り替えを繰り返す
        });

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
    })