function getQrPosition() {
    let w = 80;
    let h = 80;
    // QRコード生成
    $('.qr').qrcode({
        // render: "canvas",
        width: w,
        height: h,
        text: "this is test."
    });

    var qr_canvas = $(".qr canvas");
    console.log(qr_canvas);
    var qr_context = qr_canvas[1].getContext("2d");
    console.log(ImageProc.toBinaryData(qr_context.getImageData(0, 0, w, h).data, 2));
};