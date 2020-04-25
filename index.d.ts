interface XYZ {
    x: number
    y: number
    z: number
}

declare class Shape {
}

export const brep: {
    save(file: string, shape: Shape)
    load(file: string): Shape
    prime: {
        makeBox(p0: XYZ, p1: XYZ): Shape
    }
}

export const bool: {
    fuse(args: Shape[], tools: Shape[], opts?: { fuzzyValue?: number }): Shape
    common(args: Shape[], tools: Shape[], opts?: { }): Shape
    cut(args: Shape[], tools: Shape[], opts?: { }): Shape
    section(args: Shape[], tools: Shape[], opts?: { }): Shape
}

export const step: {
    save(file: string, shape: Shape)
    load(file: string): Shape
}
