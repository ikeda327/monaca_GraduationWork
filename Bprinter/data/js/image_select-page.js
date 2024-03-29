$(function () {
    let ip
    let context
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
            context = dst.getContext('2d')
            dst.width = 200
            dst.height = 200
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
                console.log(input_val)
                ip.threshold = input_val;
                ip.convert();
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

    // 画像選択から送信
    $("#select_finish").on("click", function () {
        let canvasdata = (ImageProc.toBinaryData(ip.cdata, 2))
        let aa = canvasdata.split('')
        // console.log("canvas : ", canvasdata)
        // console.log("aa", aa)
        let data = getQrPosition(2, aa)

        console.log(data.join('').match(/.{200}/g).join('\n'))
        // console.log(data)

        let send_data = ImageProc.hex(data)
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

        context.clearRect(0, 0, 200, 200)
        $("#qr_2").empty();
    })
});

function chgImg2() {
    var png = dst.toDataURL();
    document.getElementById("newImg2").src = png;
}