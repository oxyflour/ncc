const assert = require('assert'),
    { brep, step, Shape } = require('../'),
    { bool, builder, primitive } = brep

describe('brep', () => {
    it('should save and load brep files', () => {
        const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1])
        brep.save('build/box.brep', b1)
        const b2 = brep.load('build/box.brep')
        assert.equal(b2.type, b1.type)
    })

    describe('brep.builder', () => {
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
        it('should make boxes', () => {
            const box = primitive.makeBox([0, 0, 0], [1, 1, 1])
            assert.equal(box.find(Shape.types.FACE).length, 6)
            assert.equal(box.find(Shape.types.WIRE).length, 6)
            assert.equal(box.find(Shape.types.EDGE).length, 24)
        })
    })

    describe('brep.bool', () => {
        it('should work with fuse', () => {
            const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1]),
                b2 = primitive.makeBox([0.5, 0.5, 0.5], [1.5, 1.5, 1.5]),
                fused = bool.fuse([b1], [b2])
            assert.equal(fused.find(Shape.types.FACE).length, 12)
        })
        it('should work with common', () => {
            const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1]),
                b2 = primitive.makeBox([0.5, 0.5, 0.5], [1.5, 1.5, 1.5]),
                fused = bool.common([b1], [b2])
            assert.equal(fused.find(Shape.types.FACE).length, 6)
        })
        it('should work with cut', () => {
            const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1]),
                b2 = primitive.makeBox([0.5, 0.5, 0.5], [1.5, 1.5, 1.5]),
                fused = bool.cut([b1], [b2])
            assert.equal(fused.find(Shape.types.FACE).length, 9)
        })
        it('should work with section', () => {
            const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1]),
                b2 = primitive.makeBox([0.5, 0.5, 0.5], [1.5, 1.5, 1.5]),
                fused = bool.cut([b1], [b2])
            assert.equal(fused.find(Shape.types.FACE).length, 9)
        })
        it('should work with split', () => {
            const b1 = primitive.makeBox([0, 0, 0], [1, 1, 1]),
                b2 = primitive.makeBox([0.5, 0.5, 0.5], [1.5, 1.5, 1.5]),
                fused = bool.split([b1], [b2])
            assert.equal(fused.find(Shape.types.FACE).length, 15)
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
