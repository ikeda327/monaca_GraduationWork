// var monochrome_data;
// var send_data;

$(function () {
    let ip
    $('#myimage').on('change', async function (e) {
        // input='file' で選択した画像をプレビューに表示
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

    // 画像選択から送信
    $("#select_finish").on("click", function () {
        // console.log(monochrome_data);

        console.log(ImageProc.toBinaryData(ip.data,2))
        let send_data = ImageProc.toBinaryData(ip.data, 16)

        // console.log(send_data)
        // let send_data = ImageProc.toBinaryData(ip.data,-16)
        
        $.ajax({
            url: 'http://10.10.21.21/data',
            type: 'POST',
            // data: {data:monochrome_data},
            // data: monochrome_data.copyWithin(0,10).join('\n'),
            data: send_data,
            processData: false,
            contentType: 'text/plain',
            // dataType: 'text',
            success: function (data) {
                // alert('OK');
            },
            error: function (XMLHttpRequest, textStatus, errorThrown) {
                // alert('NG');
            }
        });
    })

});