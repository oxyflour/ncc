type Vec3 = {
    x: number
    y: number
    z: number
}
type XYZ = Vec3 | [
    number,
    number,
    number
]

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
    bound(): { min: Vec3, max: Vec3 }

    getSurfaceProps(): { mass: number }

    test(): Shape
}

export const brep: {
    save(file: string, shape: Shape)
    save(shape: Shape): Buffer
    load(file: string): Shape
    load(buffer: Buffer)
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

export const tool: {
    mesh(shapes: Shape[], xs: number[], ys: number[], zs: number[]): {
        i: number
        j: number
        k: number
        s: number
        p: Shape
    }[]
}

export const step: {
    save(file: string, shape: Shape)
    load(file: string): Shape
}
