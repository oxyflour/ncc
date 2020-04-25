function Vec3(x, y, z) {
    return { x, y, z }
}

const { brep, bool, step } = require('../'),
    b1 = brep.prime.makeBox(Vec3(0, 0, 0), Vec3(1, 1, 1)),
    b2 = brep.prime.makeBox(Vec3(0.5, 0.5, 0.5), Vec3(1.5, 1.5, 1.5)),
    c = bool.fuse([b1], [b2])
step.save('build/b1.stp', b1)
step.save('build/b2.stp', b2)
step.save('build/c.stp', c)
