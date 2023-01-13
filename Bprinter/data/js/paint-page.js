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

    //デフォルト  ペン太さ・色
    let MY_BURASISIZE = 6;
    context.strokeStyle = "#555";

    $('.paint-slider').on('input', function () {
        let val = $(this).val();
        var input_val = Number.parseInt(val);
        MY_BURASISIZE = input_val;
    });

    $('#canvas').on('touchstart', function (event) {
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
    $('#canvas').on('touchmove', function (event) {
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

    $('#canvas').on('touchend', function (event) {
        saveImageData()
    })

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
        context.globalCompositeOperation = 'source-over';
        context.strokeStyle = "#555";
        // context.lineWidth = "5"
        context.lineWidth = MY_BURASISIZE;
    });
    // 消しゴム
    $("#eraser-button").on("click ", function () {
        context.globalCompositeOperation = 'destination-out';
        context.lineWidth = MY_BURASISIZE;

    });
    // 全消去
    $("#clear-button").on("click ", function () {
        context.clearRect(0, 0, canvas.width, canvas.height);
    });

    var imageMemory = new Array(11); // キャンバスのイメージの保存用配列
    var flagMemory = 0; // 現在のキャンバスの番号
    // $('#backBtn, #forwardBtn').attr('disabled', true);
    function saveImageData() {
        // 現在の状態を保存
        if (flagMemory == imageMemory.length - 1) {
            imageMemory.shift();
        } else {
            ++flagMemory;
        }

        if (flagMemory == imageMemory.length - 1) {
            $('#forwardBtn').attr('disabled', true);
        }

        imageMemory[flagMemory] = context.getImageData(0, 0, canvas.width, canvas.height);

        $('#backBtn').attr('disabled', false);
    }
    // 戻るボタン
    $('#backBtn').click(function () {

        if (flagMemory > 0) {
            flagMemory--;
            context.putImageData(imageMemory[flagMemory], 0, 0);

            $('#forwardBtn').attr('disabled', false);
            if (flagMemory == 0) {
                $('#backBtn').attr('disabled', true);
            }
        }
    });
    // 進むボタン
    $('#forwardBtn').click(function () {
        if (flagMemory < imageMemory.length - 1) {
            flagMemory++;
            context.putImageData(imageMemory[flagMemory], 0, 0);

            $('#backBtn').attr('disabled', false);
            if (flagMemory == imageMemory.length - 1) {
                $('#forwardBtn').attr('disabled', true);
            }
        }
    });

    // ペイントから送信
    $("#paint_finish").on("click", function () {
        let canvasdata = (ImageProc.toBinaryData(context.getImageData(0, 0, w, h).data, 2))
        // canvasdata = canvasdata.replace(/\r?\n/g, "")
        canvasdata = canvasdata.split('')

        // QR埋め込み済画像 配列で返ってくる
        let data = getQrPosition(1, canvasdata)
        console.log(data.join('').match(/.{320}/g).join('\n'))

        // １６進数に変換
        // let send_data = ImageProc.toBinaryData(canvasdata, 16)
        let send_data = ImageProc.hex(canvasdata)
        console.log(send_data)

        $.ajax({
            url: '/data',
            type: 'POST',
            data: {
                'send_data': send_data
            },
            // processData: false,
            // contentType: 'text/plain',
            dataType: 'text',
            success: function (data) {},
            error: function (XMLHttpRequest, textStatus, errorThrown) {}
        })
    })

    $("#paint_finish").on('click', function() {
        context.clearRect(0, 0, canvas.width, canvas.height);
        $("#qr_1").empty();
    })
})

function chgImg1() {
    var png = canvas.toDataURL();
    document.getElementById("newImg1").src = png;

    
}