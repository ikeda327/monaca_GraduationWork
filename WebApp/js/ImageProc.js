/**
 * 
 */
 class ImageProc {
    /**
     * RGB → モノクローム変換式
     * @param {Number} r 
     * @param {Number} g 
     * @param {Number} b 
     * @returns 
     */
    static grayscale = (r, g, b) => {
        return 0.2126 * r + 0.7152 * g + 0.0722 * b
    }
    /**
     * プライベートフィールド
     */
    #ctx // CanvasRenderingContext2D
    #src // ImageData カラー画像 HTMLCanvasElement のサイズにリサイズ済み
    #dst // ImageData 表示画像
    /**
     * 白/黒 変換のしきい値
     * private 
     */
    #thr = 0
    /**
     * コンストラクタ
     * 
     * @param {HTMLCanvasElement} canvas    表示するCanvasエレメント、出力サイズに設定しておく
     * @param {HTMLImageElement} image      変換元画像
     */
    constructor(canvas, image) {
        // リサイズ用データ
        let is = {
            x: 0,
            y: 0,
            width: canvas.width,
            height: canvas.height
        }
        this.#ctx = canvas.getContext("2d");
        if (image != undefined){
            this.#resize(is, canvas.width)
            this.#ctx.drawImage(image, 0, 0, image.width, image.height, is.x, is.y, is.width, is.height);
        }

        // ブレークポイント
        this.#src = this.#ctx.getImageData(0, 0, canvas.width, canvas.height)
        this.#dst = this.#ctx.createImageData(canvas.width, canvas.height)
        // カラー画像を元にしきい値を計算する
        this.#thr = this.calcThreshold()
    }
    /**
     * sizeで指定された範囲におさまるサイズを計算する
     * 画面中央に配置されるように開始位置も計算する
     * @param {リサイズ用データ} is 
     * @param {Number} size 
     */
    #resize(is, size) {
        let l = is.width > is.height ? is.width : is.height
        if (l > size) {
            let coefficient = size / l
            is.width *= coefficient
            is.height *= coefficient
        }
        is.x = (size - is.width) / 2
        is.y = (size - is.height) / 2
    }
    /**
     * 2値化する際のしきい値
     * threshold getプロパティ
     */
    get threshold() {
        return this.#thr
    }
    /**
     * 2値化する際のしきい値
     * threshold setプロパティ
     * @param {Number} arg 値の範囲 0～255
     */
    set threshold(arg) {
        if (arg != undefined && arg.constructor.name == 'Number' && arg >= 0 && arg < 256) {
            this.#thr = arg
        }
    }
    refresh(){
        this.#ctx = canvas.getContext("2d");
        this.#src = this.#ctx.getImageData(0, 0, canvas.width, canvas.height)
        this.#dst = this.#ctx.createImageData(canvas.width, canvas.height)
    }
    /**
     * カラー画像を表示する
     */
    drawOriginal() {
        this.#ctx.putImageData(this.#src, 0, 0)
    }
    /**
     * モノクローム画像に変換し表示する
     */
    convert() {
        let v;
        let data = "";
        monochrome_data ="";
        for (let i = 0; i < this.#dst.data.length; i += 4) {
            v = ImageProc.grayscale(this.#src.data[i], this.#src.data[i + 1], this.#src.data[i + 2])
            if (this.#src.data[i + 3] < 255) {
                v *= this.#src.data[i + 3] / 255
            }
            // console.log("alpha:",this.#src.data[i + 3],",value:",v)
            let img_data;   // ０ or １
            if (this.#src.data[i + 3] == 0 && v == 0) {
                // 透明の場合白とする
                v = 255
                img_data = 0
                data += img_data //+ ' ';
            } else {
                if (v < this.#thr) {
                    v = 0
                    img_data = 1
                    data += img_data //+ ' ';
                } else {
                    v = 255
                    img_data = 0
                    data += img_data //+ ' ';
                }
            }
            // RGB同じ値とする
            this.#dst.data[i] = this.#dst.data[i + 1] = this.#dst.data[i + 2] = v
            // 不透明
            this.#dst.data[i + 3] = 255
        }
        // 白黒 0,1データ（文字列）
        // const data_320 = data.match(/.{640}/g);
        const data_320 = data.match(/.{320}/g);
        monochrome_data = data_320;
        var str = "";
        for( let n = 0; n < data_320.length; n++ ) {
            str += data_320[n] + "\\n"; 
        }

        monochrome_data = data_320.join('\n')
        console.log(monochrome_data)

        // 16進数
        let byte = 0
        let bytes = new Array()
        let index = 0
        str.split("").forEach(e => {
            if (e == '\n') {
                bytes.push(byte.toString(16).padStart(2,'0'))
                bytes.push(e)
                index = 0
                byte = 0
            } else {
                byte <<= 1
                byte |= Number.parseInt(e)
                index++
                if (index > 7) {
                    bytes.push(byte.toString(16).padStart(2,'0'))
                    index = 0
                    byte = 0
                }
            }
        })
        // send_data = bytes.join("");
        // console.log(send_data);

        this.#ctx.putImageData(this.#dst, 0, 0)
        return bytes
    }
    /**
     * 現在のカラー画像を元に2値化する際のしきい値を計算する
     */
    calcThreshold() {
        let histgram = Array(256).fill(0)
        let t = 0
        let max = 0
        // 明るさの分布を集計する
        for (let i = 0; i < this.#src.data.length; i += 4) {
            let g = ~~ImageProc.grayscale(this.#src.data[i], this.#src.data[i + 1], this.#src.data[i + 2])
            histgram[g]++
        }
        for (let i = 0; i < 256; i++) {
            let w1 = 0
            let w2 = 0
            let sum1 = 0
            let sum2 = 0
            let m1 = 0
            let m2 = 0
            for (let j = 0; j <= i; ++j) {
                w1 += histgram[j]
                sum1 += j * histgram[j]
            }
            for (let j = i + 1; j < 256; ++j) {
                w2 += histgram[j]
                sum2 += j * histgram[j]
            }
            if (w1) {
                m1 = sum1 / w1
            }
            if (w2) {
                m2 = sum2 / w2
            }
            let tmp = (w1 * w2 * (m1 - m2) * (m1 - m2))
            if (tmp > max) {
                max = tmp
                t = i
            }
        }
        return t
    }
}
