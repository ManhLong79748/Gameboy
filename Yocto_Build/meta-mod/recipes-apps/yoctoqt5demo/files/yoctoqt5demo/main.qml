import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 320  // Portrait width
    height: 240 // Height
    title: qsTr("Hello World")

    Rectangle {
        id: rect
        color: "red"
        anchors.fill: parent

        Canvas {
            id: starCanvas
            width: 150
            height: 150
            anchors.horizontalCenter: parent.horizontalCenter  // Explicit center để fix lệch
            anchors.verticalCenter: parent.verticalCenter

            property int spikes: 5
            property real outerRatio: 0.9  // To hơn để center rõ
            property real innerRatio: 0.4  // Adjust để thẳng, không nghiêng

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);

                var cx = width / 2;
                var cy = height / 2;
                var outer = Math.min(width, height) / 2;
                var inner = outer * starCanvas.innerRatio;
                var rot = 0;  // Bắt đầu từ 0 để thẳng (không nghiêng)
                var step = Math.PI * 2 / spikes;  // Step đầy đủ để closed path chuẩn

                ctx.beginPath();
                ctx.moveTo(cx + Math.cos(rot) * outer, cy + Math.sin(rot) * outer);
                for (var i = 1; i <= spikes; i++) {  // Loop <= spikes để close
                    var rotInner = rot + step / 2;
                    ctx.lineTo(cx + Math.cos(rotInner) * inner, cy + Math.sin(rotInner) * inner);
                    rot += step;
                    ctx.lineTo(cx + Math.cos(rot) * outer, cy + Math.sin(rot) * outer);
                }
                ctx.closePath();

                ctx.fillStyle = "#FFD700";  // Gold yellow explicit để màu đúng
                ctx.fill();
            }

            Component.onCompleted: requestPaint()

            onWidthChanged: requestPaint()
            onHeightChanged: requestPaint()

            // Bỏ Connections nếu không cần, hoặc giữ với qualify
            Connections {
                target: rect
                onWidthChanged: starCanvas.requestPaint()
                onHeightChanged: starCanvas.requestPaint()
            }
        }
    }
}