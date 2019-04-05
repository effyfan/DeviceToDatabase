const { Pool } = require('pg');

const pool = new Pool({
    connectionString: process.env.CONNECTION_STRING
});

async function insertRow(event) {
    const query = `INSERT INTO environment
        (device, temperature, humidity, sound, motion, vibration, light)
        VALUES ($1,$2,$3,$4,$5,$6,$7)`;
    const values = [event.device, event.temperature, event.humidity, event.sound, event.motion, event.vibration, event.light];
    console.log(query, values);

    const result = await pool.query(query, values);
    return result.rowCount;
};

exports.handler = async (event) => {
    await insertRow(event);
    const response = {
        statusCode: 200,
        body: 'OK'
    };
    return response;
};
