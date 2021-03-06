var AWS = require('aws-sdk');
AWS.config.update({ region: 'us-east-1' });

const mqtt = require('mqtt');
const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

mqttClient.on('connect', () => {
    console.log('MQTT Connected');
    mqttClient.subscribe('itp/device_xx/temperature');
});

mqttClient.on('message', (topic, message) => {
    console.log(topic, message.toString());
    const temperature = Number(message.toString());

    if (temperature > 80) {
        const alertMessage = `Temperature ${temperature}°F exceeds the high temperature limit of 80°F`
        // mqttClient.publish('itp/device_xx/alert', alertMessage);
        sendSms('+16265660998', alertMessage);
    }
});

async function sendSms(number, message) {

    var params = {
        Message: message,
        PhoneNumber: number
    };

    try {
        // Send the message
        let response = await new AWS.SNS({ apiVersion: '2010-03-31' }).publish(params).promise();
        console.log(`MessageID is ${response.MessageId}`);
    } catch (e) {
        console.error(e, e.stack)
    }
}
