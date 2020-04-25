interface XYZ {
    x: number
    y: number
    z: number
}

class Box {
    constructor(public p0: XYZ, public p1: XYZ)
}

const prime = {
    Box
}

export const brep = {
    prime
}
