$(function () {
    // QRコード生成
    $('.qr').qrcode({
        // render: "canvas",
        width: 70,
        height: 70,
        text: "this is test."
    });
})

class QRcode {
    // QRコードの座標取得
    getPosition() {
        var qr_canvas = $(".qr canvas");
        var qr_context = qr_canvas.getContext("2d");
        var w = $('.qr').width();
        var h = $('.qr').height();
        console.log(ImageProc.toBinaryData(qr_context.getImageData(0, 0, w, h).data, 2))
    };
}