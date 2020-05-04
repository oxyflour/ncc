type XYZ = {
    x: number
    y: number
    z: number
} | [number, number, number]

declare enum ShapeType {
    COMPOUND,
    COMPSOLID,
    EDGE,
    FACE,
    SHAPE,
    SHELL,
    SOLID,
    VERTEX,
    WIRE,
}

declare class Shape {
    static types: typeof ShapeType
    type: ShapeType
    find(type: ShapeType): Shape[]
    test(): Shape
}

export const brep: {
    save(file: string, shape: Shape)
    load(file: string): Shape
    builder: {
        makeVertex(p0: XYZ): Shape
        makeEdge(p0: XYZ, p1: XYZ): Shape
        makeFace(pos: XYZ, dir: XYZ): Shape
        makeFace(wire: Shape): Shape
        makeWire(edges: Shape[]): Shape
        makeShell(wires: Shape[]): Shape
        makeCompound(shapes: Shape[]): Shape
        makeSolid(shapes: Shape[]): Shape
        toNurbs(shape: Shape): Shape
    }
    primitive: {
        makeSphere(p: XYZ, r: number): Shape
        makeBox(p0: XYZ, p1: XYZ): Shape
    }
    bool: {
        fuse(args: Shape[], tools: Shape[], opts?: { fuzzyValue?: number }): Shape
        common(args: Shape[], tools: Shape[], opts?: { }): Shape
        cut(args: Shape[], tools: Shape[], opts?: { }): Shape
        section(args: Shape[], tools: Shape[], opts?: { }): Shape
        split(args: Shape[], tools: Shape[], opts?: { }): Shape
    }
}


export const step: {
    save(file: string, shape: Shape)
    load(file: string): Shape
}
