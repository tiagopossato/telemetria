var medidorTensao = new JustGage({
    id: "medidorTensao",
    value: 0,
    min: 5,
    max: 50,
    title: "Tensão",
    label: "Volts",
    pointer: true,
    relativeGaugeSize: true,
    decimals: true,
    customSectors: [{
            color: '#ff0000',
            lo: 5,
            hi: 9
        }, {
            color: '#318BEB',
            lo: 9,
            hi: 20
        }, {
            color: '#31EB41',
            lo: 20,
            hi: 40
        }]
});

var medidorCorrente = new JustGage({
    id: "medidorCorrente",
    value: 0,
    min: 0,
    max: 30,
    title: "Corrente",
    label: "Amperes",
    pointer: true,
    relativeGaugeSize: true,
	decimals: true,
    customSectors: [{
            color: '#31EB41',
            lo: 0,
            hi: 5
        }, {
            color: '#318BEB',
            lo: 10,
            hi: 15
        }, {
            color: '#ff0000',
            lo: 15,
            hi: 30
        }]
});

var temperaturaBaterias = new JustGage({
    id: "temperaturaBaterias",
    value: 0,
    min: 0,
    max: 100,
    title: "Baterias",
    label: "°C",
    pointer: true,
    relativeGaugeSize: true,
	decimals: true,
    customSectors: [{
            color: '#31EB41',
            lo: 0,
            hi: 40
        }, {
            color: '#318BEB',
            lo: 40,
            hi: 55
        }, {
            color: '#ff0000',
            lo: 55,
            hi: 100
        }]
});

var temperaturaCockpit = new JustGage({
    id: "temperaturaCockpit",
    value: 0,
    min: 0,
    max: 100,
    title: "Cockpit",
    label: "°C",
    pointer: true,
    relativeGaugeSize: true,
	decimals: true,
    customSectors: [{
            color: '#31EB41',
            lo: 0,
            hi: 40
        }, {
            color: '#318BEB',
            lo: 40,
            hi: 55
        }, {
            color: '#ff0000',
            lo: 55,
            hi: 100
        }]
});



var velocimetro = new JustGage({
    id: "velocimetro",
    value: 0,
    min: 0,
    max: 50,
    title: "Velocidade",
    label: "Km/h",
    pointer: true,
    relativeGaugeSize: true,
	decimals: true,
    customSectors: [{
            color: '#31EB41',
            lo: 0,
            hi: 25
        }, {
            color: '#318BEB',
            lo: 25,
            hi: 35
        }, {
            color: '#ff0000',
            lo: 35,
            hi: 45
        }]
});

var wattimetro = new JustGage({
    id: "wattimetro",
    value: 0,
    min: 0,
    max: 1500,
    title: "Potência Instantânea",
    label: "W",
    pointer: true,
    relativeGaugeSize: true,
	decimals: true,
    customSectors: [{
            color: '#31EB41',
            lo: 0,
            hi: 200
        }, {
            color: '#318BEB',
            lo: 201,
            hi: 500
        }, {
            color: '#ff0000',
            lo: 501,
            hi: 1500
        }]
});

