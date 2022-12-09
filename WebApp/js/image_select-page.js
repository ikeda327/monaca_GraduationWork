$(function () {
    let ip
    $('#myimage').on('change', async function (e) {
        // 選択した画像をプレビューに表示
        var reader = new FileReader();
        reader.onload = function (e) {
            $("#photo").attr('src', e.target.result);
            after_read(e.target.result);
        }
        let src = reader.readAsDataURL(e.target.files[0]);

        // 白黒化　
        const image = new Image()

        function after_read(_src) {
            // console.log(_src);
            image.src = _src;
            let dst = document.getElementById("dst")
            dst.width = 320
            dst.height = 320
            image.onload = () => {
                ip = new ImageProc(dst, image)
                let element = document.getElementById('input');
                element.value = ip.threshold;
                let val = $('#input').val();
                $('.value').html(val);
                ip.threshold = ip.calcThreshold();
                ip.convert();
            }
            // 白黒 調節バー
            $('.slider').on('input', function () {
                let val = $(this).val();
                $('.value').html(val);
                var input_val = Number.parseInt(val);
                ip.threshold = input_val;
                send_data = ip.convert();
            });
            // 自動ボタン
            $('#reload_btn').on('click', function () {
                ip.threshold = ip.calcThreshold();
                ip.convert();
                $('.value').html(ip.threshold);
            })
        }
        // ip.drawOriginal()    元画像を表示する
        // ip.convert() //モノクローム画像に変換する
        // ip.threshold = 200   しきい値を200とする
        // ip.threshold = ip.calcThreshold()    計算したしきい値を指定する
    });

    function getQrPosition() {
        let w = 70;
        let h = 70;
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

    // 画像選択から送信
    $("#select_finish").on("click", function () {
        getQrPosition();

        console.log(ImageProc.toBinaryData(ip.data, 2))
        let send_data = ImageProc.toBinaryData(ip.data, 16)

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
        });
    })

});