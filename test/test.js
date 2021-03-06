const assert = require('assert'),
    { brep, step, tool, Shape } = require('../'),
    { bool, builder, primitive } = brep

describe('shape', () => {
    it('shape.bound', () => {
        const b1 = primitive.makeBox([1, 2, 3], [4, 5, 6])
        assert.deepEqual(b1.bound(), {
            min: { x: 0.9999999, y: 1.9999999, z: 2.9999999 },
            max: { x: 4.0000001, y: 5.0000001, z: 6.0000001 },
        })
    })

    it('shape.getSurfaceProps', () => {
        const b1 = primitive.makeBox([0, 0, 0], [1, 2, 3]),
            [f1] = b1.find(Shape.types.FACE)
        assert.equal(f1.getSurfaceProps().mass, 6)
    })
})

describe('brep', () => {
    it('should save and load brep files', () => {
        const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1])
        brep.save('build/box.brep', b1)
        const b2 = brep.load('build/box.brep')
        assert.equal(b2.type, b1.type)

        const buf = brep.save(b1)
        assert.equal(Buffer.isBuffer(buf), true)
        const b3 = brep.load(buf)
        assert.equal(b2.type, b3.type)
    })

    describe('brep.builder', () => {
        it('should make edge', () => {
            const edge = builder.makeEdge([0, 0, 0], [0, 0, 1])
            assert.equal(edge.type, Shape.types.EDGE)
        })
        it('should make face', () => {
            const face = builder.makeFace([0, 0, 0], [1, 1, 1])
            assert.equal(face.type, Shape.types.FACE)
        })
        it('should make compound', () => {
            const comp = builder.makeCompound([
                builder.makeFace([0, 0, 0], [1, 1, 1]),
                builder.makeFace([0, 0, 0], [1, 1, 1]),
            ])
            assert.equal(comp.type, Shape.types.COMPOUND)
        })
    })

    describe('brep.primitive', () => {
        it('should make sphere', () => {
            const sphere = primitive.makeSphere([0, 0, 0], 1)
            assert.equal(sphere.find(Shape.types.FACE).length, 1)
            assert.equal(sphere.find(Shape.types.WIRE).length, 1)
            assert.equal(sphere.find(Shape.types.EDGE).length, 4)
        })
        it('should make boxes', () => {
            const box = primitive.makeBox([0, 0, 0], [1, 1, 1])
            assert.equal(box.find(Shape.types.FACE).length, 6)
            assert.equal(box.find(Shape.types.WIRE).length, 6)
            assert.equal(box.find(Shape.types.EDGE).length, 24)
        })
    })

    describe('brep.bool', () => {
        const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1]),
            b2 = primitive.makeBox([0.5, 0.5, 0.5], [1.5, 1.5, 1.5])
        it('should work with fuse', () => {
            const ret = bool.fuse([b1], [b2])
            assert.equal(ret.find(Shape.types.FACE).length, 12)
        })
        it('should work with common', () => {
            const ret = bool.common([b1], [b2])
            assert.equal(ret.find(Shape.types.FACE).length, 6)
        })
        it('should work with cut', () => {
            const ret = bool.cut([b1], [b2])
            assert.equal(ret.find(Shape.types.FACE).length, 9)
        })
        it('should work with section', () => {
            const ret = bool.cut([b1], [b2])
            assert.equal(ret.find(Shape.types.FACE).length, 9)
        })
        it('should work with split', () => {
            const ret = bool.split([b1], [b2])
            assert.equal(ret.find(Shape.types.FACE).length, 15)
        })
    })
})

describe('step', () => {
    it('should save and load brep files', () => {
        const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1])
        step.save('build/box.stp', b1)
        const b2 = step.load('build/box.stp')
        assert.equal(b2.type, b1.type)
    })
})

describe('tool', () => {
    it('tool.mesh', () => {
        const b1 = primitive.makeBox([0, 0, 0], [1.1, 1.1, 1.1]),
            mesh = tool.mesh([b1], [0.5, 0.5], [-0.5, 0, 1, 1.5], [-0.5, 0, 1, 1.5])
        assert.deepEqual(
            mesh.map(({ i, j, k, s }) => ({ i, j, k, s })), [
            { i: 0, j: 1, k: 1, s: 1 },
            { i: 0, j: 1, k: 2, s: 0.10000000000000009 },
            { i: 0, j: 2, k: 1, s: 0.10000000000000009 },
            { i: 0, j: 2, k: 2, s: 0.010000000000000018 }
        ])
    })
})
