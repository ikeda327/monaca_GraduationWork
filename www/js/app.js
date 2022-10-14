window.onload = function () {
    // キャンバス機能
    var canvas = $('#canvas')[0];
    var context = canvas.getContext('2d');
    var startX, startY;

    var w = $('.paint-canvas').width();
    var h = $('.paint-canvas').height();
    $('#canvas').attr('width', w);
    $('#canvas').attr('height', h);

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
        context.strokeStyle = "#000 ";
        context.lineWidth = "1 "
    });

    // 消しゴム
    $("#eraser-button").on("click ", function () {
        context.strokeStyle = "#fff ";
        context.lineWidth = "5 "
    });

    // 全消去
    $("#clear-button").on("click ", function () {
        context.clearRect(0, 0, canvas.width, canvas.height);
    });

};

// class ImageProc {
//     /**
//      * メイン
//      * @param {Object} canvas
//      * @param {Object} image
//      */
//     main(canvas, image) {
//         const grayscale = (r, g, b) => 0.2126 * r + 0.7152 * g + 0.0722 * b
//         let ctx = canvas.getContext("2d");
//         ctx.drawImage(image, 0, 0, image.width, image.height)
//         let src = ctx.getImageData(0, 0, image.width, image.height)
//         let dst = ctx.createImageData(image.width, image.height)

//         let t = this.threshold(src)
//         console.log(t);
//         t *= 0.6;

//         for (let i = 0; i < dst.data.length; i += 4) {
//             let v = grayscale(src.data[i], src.data[i + 1], src.data[i + 2])
//             if (v < t) {
//                 dst.data[i] = dst.data[i + 1] = dst.data[i + 2] = 0
//             } else {
//                 dst.data[i] = dst.data[i + 1] = dst.data[i + 2] = 255
//             }
//             dst.data[i + 3] = 255
//         }
//         ctx.putImageData(dst, 0, 0)
//     }
//     /**
//      * 大津の2値化
//      * @param {ImageData} src
//      */
//     threshold(src) {
//         const grayscale = (r, g, b) => 0.2126 * r + 0.7152 * g + 0.0722 * b
//         // 黒：白 比率
//         let histgram = Array(256).fill(0)
//         let t = 0
//         let max = 0

//         for (let i = 0; i < src.data.length; i += 4) {
//             let g = ~~grayscale(src.data[i], src.data[i + 1], src.data[i + 2])
//             histgram[g]++
//         }

//         for (let i = 0; i < 256; i++) {
//             let w1 = 0
//             let w2 = 0
//             let sum1 = 0
//             let sum2 = 0
//             let m1 = 0
//             let m2 = 0
//             for (let j = 0; j <= i; ++j) {
//                 w1 += histgram[j]
//                 sum1 += j * histgram[j]
//             }
//             for (let j = i + 1; j < 256; ++j) {
//                 w2 += histgram[j]
//                 sum2 += j * histgram[j]
//             }
//             if (w1) {
//                 m1 = sum1 / w1
//             }
//             if (w2) {
//                 m2 = sum2 / w2
//             }
//             let tmp = (w1 * w2 * (m1 - m2) * (m1 - m2))
//             if (tmp > max) {
//                 max = tmp
//                 t = i
//             }
//         }
//         return t
//     }
// }

// 選択ページ
// カメラ・ギャラリー
function snapPicture(mode) {
    var options;

    if (mode == "camera") {
        options = {
            // 標準のカメラを起動
            sourceType: Camera.PictureSourceType.CAMERA,
            // データの形式
            destinationType: Camera.DestinationType.DATA_URL
        }
    } else if (mode == "galary") {
        options = {
            // ライブラリを起動
            sourceType: Camera.PictureSourceType.PHOTOLIBRARY,
            destinationType: Camera.DestinationType.DATA_URL
        }
    }

    // カメラ起動
    navigator.camera.getPicture(cameraSuccess, cameraError, options);

    function cameraSuccess(imageData) {
        // 撮影、選択した画像を画面に表示
        var img = document.querySelector("#photo");
        img.src = "data:image/jpeg;base64," + imageData;

        const image = new Image()
        var ip
        window.onload = () => {
            let dst = document.getElementById("dst")
            dst.width = 350
            dst.height = 350

            image.onload = () => {
                ip = new ImageProc(dst, image)
            }

            // ip.drawOriginal()    元画像を表示する
            ip.convert() //モノクローム画像に変換する
            // ip.threshold = 200   しきい値を200とする
            // ip.threshold = ip.calcThreshold()    計算したしきい値を指定する
        }
    }

    function cameraError(message) {
        console.log(message)
    }



}