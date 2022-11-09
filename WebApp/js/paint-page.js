// var monochrome_data;
// var send_data;
$(function () {
    $(window).on('touchmove.noScroll', function (e) {
        e.preventDefault();
    });
    // キャンバス機能
    let canvas = $('#canvas')[0];
    let context = canvas.getContext('2d');
    let startX, startY;
    let w = $('.paint-canvas').width();
    let h = $('.paint-canvas').height();
    // let paint_ip = new ImageProc(canvas)
    $('#canvas').attr('width', w);
    $('#canvas').attr('height', h);
    $('canvas').on('touchstart', function (event) {
        // 画面がタッチされたときの処理を記述する
        event.preventDefault();
        let pageX = event.originalEvent.touches[0].pageX;
        let pageY = event.originalEvent.touches[0].pageY;
        // タッチポイントからキャンバスの位置を差し引いてキャンバス座標に変え、
        // 描画の開始位置とする
        let point = getCanvasPoint(pageX, pageY);
        startX = point.x;
        startY = point.y;
    });
    $('canvas').on('touchmove', function (event) {
        // 画面がタッチされながら移動したときの処理を記述する
        event.preventDefault();
        let pageX = event.originalEvent.touches[0].pageX;
        let pageY = event.originalEvent.touches[0].pageY;
        let point = getCanvasPoint(pageX, pageY);
        let endX = point.x;
        let endY = point.y;
        // 直線を描画
        context.lineWidth = "5"
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
        let base = canvas.getBoundingClientRect();
        return {
            x: pageX - base.left,
            y: pageY - base.top
        };
    }
    // ペン
    $("#pen-button").on("click ", function () {
        context.strokeStyle = "#000";
        context.lineWidth = "5"
    });
    // 消しゴム
    $("#eraser-button").on("click ", function () {
        context.strokeStyle = "#fff";
        context.lineWidth = "10";
    });
    // 全消去
    $("#clear-button").on("click ", function () {
        context.clearRect(0, 0, canvas.width, canvas.height);
    });

    // ペイントから送信
    $("#paint_finish").on("click", function () {
        // paint_ip.refresh()
        let paint_ip = new ImageProc(canvas)
        send_data = paint_ip.convert()

        // $.ajax({
        //     url: 'http://10.10.21.21/data',
        //     type: 'POST',
        //     data: send_data,
        //     processData: false,
        //     contentType: 'text/plain',
        //     // dataType: 'text',
        //     success: function (data) {
        //         // alert('OK');
        //     },
        //     error: function (XMLHttpRequest, textStatus, errorThrown) {
        //         // alert('NG');
        //     }
        // });
    })
})