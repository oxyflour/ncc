function XYZ(x, y, z) {
    return { x, y, z }
}

const { brep, step, Shape } = require('../'),
    { bool, prime, builder } = brep,
    b1 = prime.makeBox(XYZ(0, 0, 0), XYZ(1, 1, 1)),
    b2 = prime.makeBox(XYZ(0.5, 0.5, 0.5), XYZ(1.5, 1.5, 1.5))
step.save('build/fuse.stp', bool.fuse([b1], [b2]))
step.save('build/common.stp', bool.common([b1], [b2]))
step.save('build/cut.stp', bool.cut([b1], [b2]))
step.save('build/section.stp', bool.section([b1], [b2]))
step.save('build/split.stp', bool.split([b1], [b2]))

const c = new Shape([b1, b2])
brep.save('build/c.brep', c)
console.log(c.type, Shape.types.COMPOUND)

console.log(b1.find(Shape.types.FACE))

const f = builder.makeFace(XYZ(0.5, 0.5, 0.5), XYZ(1, 1, 1))
step.save('build/clip.stp', bool.split([b1], [f]))
