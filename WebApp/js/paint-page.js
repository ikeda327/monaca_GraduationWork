$(function () {
    $(window).on('touchmove.noScroll', function (e) {
        e.preventDefault();
    });
    // キャンバス機能
    var canvas = $('#canvas')[0];
    // var context = canvas.getContext('2d');
    var context = canvas.getContext('2d', {
        alpha: true,
        desynchronized: false,
        colorSpace: "srgb",
        willReadFrequently: true
    });
    var startX, startY;
    // ↓移動
    // let paint_ip = new ImageProc(canvas);
    var w = $('.paint-canvas').width();
    var h = $('.paint-canvas').height();

    $('#canvas').attr('width', w);
    $('#canvas').attr('height', h);

    // →移動
    // let paint_ip = new ImageProc(canvas)

    //デフォルトのペン太さ
    let MY_BURASISIZE = 6;

    $('.paint-slider').on('input', function () {
        let val = $(this).val();
        var input_val = Number.parseInt(val);
        MY_BURASISIZE = input_val;
    });

    $('canvas').on('touchstart', function (event) {
        // 画面がタッチされたときの処理を記述する
        event.preventDefault();
        var pageX = event.originalEvent.touches[0].pageX;
        var pageY = event.originalEvent.touches[0].pageY;
        // タッチポイントからキャンバスの位置を差し引いてキャンバス座標に変え、
        // 描画の開始位置とする
        var point = getCanvasPoint(pageX, pageY);
        startX = point.x;
        startY = point.y;
    });
    $('canvas').on('touchmove', function (event) {
        // 画面がタッチされながら移動したときの処理を記述する
        event.preventDefault();
        var pageX = event.originalEvent.touches[0].pageX;
        var pageY = event.originalEvent.touches[0].pageY;
        var point = getCanvasPoint(pageX, pageY);
        var endX = point.x;
        var endY = point.y;
        // 直線を描画
        context.lineWidth = MY_BURASISIZE;
        // context.lineWidth = "5"
        context.lineCap = "round";
        context.beginPath();
        context.moveTo(startX, startY);
        context.lineTo(endX, endY);
        context.stroke();
        // 次の開始位置を変数に入れてメモしておく
        startX = endX;
        startY = endY;
    });
    // 画面のx,y座標からcanvasのx,y座標に変換する
    function getCanvasPoint(pageX, pageY) {
        var base = canvas.getBoundingClientRect();
        return {
            x: pageX - base.left,
            y: pageY - base.top
        };
    }
    // ペン
    $("#pen-button").on("click ", function () {
        context.strokeStyle = "#000";
        // context.lineWidth = "5"
        context.lineWidth = MY_BURASISIZE;
    });
    // 消しゴム
    $("#eraser-button").on("click ", function () {
        context.strokeStyle = "#fff";
        // context.lineWidth = "10";       
        context.lineWidth = MY_BURASISIZE;

    });
    // 全消去
    $("#clear-button").on("click ", function () {
        context.clearRect(0, 0, canvas.width, canvas.height);
    });

    // ペイントから送信
    $("#paint_finish").on("click", function () {
        let send_data = ImageProc.toBinaryData(context.getImageData(0, 0, w, h).data, 16)
        // console.log(ImageProc.toBinaryData(context.getImageData(0, 0, w, h).data, 2))

        // paint_ip.refresh()
        // send_data = paint_ip.convert()
        // var image = new Image()
        // console.log(send_data)

        // $.ajax({
        //     url: 'http://10.10.21.21/data',
        //     type: 'POST',
        //     data: send_data,
        //     processData: false,
        //     contentType: 'text/plain',
        //     // dataType: 'text',
        //     success: function (data) {
        //     },
        //     error: function (XMLHttpRequest, textStatus, errorThrown) {
        //     }
        // });
    })
})