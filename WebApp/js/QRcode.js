// $(function () {
// let w = 70;
// let h = 70;
// // QRコード生成
// $('.qr').qrcode({
//     // render: "canvas",
//     width: w,
//     height: h,
//     text: "this is test."
// });

// class QRcode {
//     // QRコードの座標取得
//     getQrPosition() {
//         var qr_canvas = $(".qr canvas");
//         var qr_context = qr_canvas.getContext("2d");
//         // console.log(qr_context.getImageData(0, 0, w, h).data, 2);
//         console.log(ImageProc.toBinaryData(qr_context.getImageData(0, 0, w, h).data, 2))
//     };
// }