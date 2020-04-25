interface XYZ {
    x: number
    y: number
    z: number
}

class Shape {
}

export const brep: {
    prime: {
        makeBox(p0: XYZ, p1: XYZ): Shape
    }
}

export const bool: {
    fuse(args: Shape[], tools: Shape[], opts?: { fuzzyValue?: number }): Shape
}

export const step: {
    save(file: string, shape: Shape)
    load(file: string): Shape
}
