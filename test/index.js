const { brep } = require('../'),
    box = new brep.prime.Box({ x: 0, y: 0, z: 0 }, { x: 1, y: 1, z: 1 })
console.log(box.p0, box.p1)
