function Vec3(x, y, z) {
    return { x, y, z }
}

const { brep, step, Shape } = require('../'),
    { bool, prime } = brep,
    b1 = prime.makeBox(Vec3(0, 0, 0), Vec3(1, 1, 1)),
    b2 = prime.makeBox(Vec3(0.5, 0.5, 0.5), Vec3(1.5, 1.5, 1.5))
step.save('build/fuse.stp', bool.fuse([b1], [b2]))
step.save('build/common.stp', bool.common([b1], [b2]))
step.save('build/cut.stp', bool.cut([b1], [b2]))
step.save('build/section.stp', bool.section([b1], [b2]))
step.save('build/split.stp', bool.split([b1], [b2]))

const c = new Shape([b1, b2])
brep.save('build/c.brep', c)
console.log(c.type, Shape.types.COMPOUND)

console.log(b1.find(Shape.types.FACE))
