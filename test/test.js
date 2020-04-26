const assert = require('assert'),
    { brep, step, Shape } = require('../'),
    { bool, builder, prime } = brep

describe('brep', () => {
    it('should save and load brep files', () => {
        const b1 = prime.makeBox([0, 0, 0], [1, 1, 1])
        brep.save('build/box.brep', b1)
        const b2 = brep.load('build/box.brep')
        assert.equal(b2.type, b1.type)
    })

    describe('brep.bool', () => {
        it('should work with fuse', () => {
            const b1 = prime.makeBox([0, 0, 0], [1, 1, 1]),
                b2 = prime.makeBox([0.5, 0.5, 0.5], [1.5, 1.5, 1.5]),
                fused = bool.fuse([b1], [b2])
            assert.equal(fused.find(Shape.types.FACE).length, 12)
        })
    })
})

describe('step', () => {
    it('should save and load brep files', () => {
        const b1 = prime.makeBox([0, 0, 0], [1, 1, 1])
        step.save('build/box.stp', b1)
        const b2 = step.load('build/box.stp')
        assert.equal(b2.type, b1.type)
    })
})
