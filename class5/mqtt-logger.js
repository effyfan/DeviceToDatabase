const mqtt = require('mqtt');
const fs = require('fs');
const fileName = './mqtt.log';

const mqttClient = mqtt.connect(process.env.MQTT_SERVER);

mqttClient.on('connect', () => {
    console.log('MQTT Connected');
    // mqttClient.subscribe('#');
    mqttClient.subscribe('itp/device_ef1/+');
    mqttClient.subscribe('itp/device_ef2/+');
    mqttClient.subscribe('itp/device_ef3/+');
    mqttClient.subscribe('itp/device_ef4/+');
});

mqttClient.on('message', function (topic, message) {

    let payload = message.toString();
    // replace tabs with 2 spaces
    payload = payload.replace(/\t/g, '  ');
    // replace linefeeds with 1 space
    payload = payload.replace(/\n/g, ' ');

    const timestamp = new Date().getTime();
    const data = `${timestamp}\t${topic}\t${payload}\n`;
    fs.writeFile(fileName, data, { flag: 'a' }, () => {});
});

// const payload = message.toString();
// // replace tabs with 2 spaces
// payload = payload.replace(/\t/g, '  ');
// // replace linefeeds with 1 space
// payload = payload.replace(/\n/g, ' ');
