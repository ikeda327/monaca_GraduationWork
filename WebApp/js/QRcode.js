function getQrPosition(canvasdata) {
    let w = 80;
    let h = 80;
    // QRコード生成
    $('.qr').qrcode({
        // render: "canvas",
        width: w,
        height: h,
        text: "https://www.google.com"
    });

    let qr_canvas = $(".qr canvas");
    let qr_context = qr_canvas[1].getContext("2d");
    let qrdata = (ImageProc.toBinaryData(qr_context.getImageData(0, 0, w, h).data, 2));
    qrdata = qrdata.replace(/\r?\n/g, "")
    // ８０行ずつの文字列にする
    // console.log(qrdata.match(/.{80}/g).join('\n'))

    qrdata = qrdata.split('')
    // console.log(qrdata)


    for (let qy = 0; qy < 80; qy++) {
        for (let qx = 0; qx < 80; qx++) {
            canvasdata[(320 * (240 + qy)) + (240 + qx)] = qrdata[80 * qy + qx]
        }
    }
    return canvasdata
};