#pragma once
#include <engine/Mesh.h>
#include <vector>
#include <array>
#include <optional>
#include <tuple>
#include <cmath>

namespace MeshFactory
{

    static constexpr float PI_F = 3.14159265358979323846f;

    // ----------------------------- Helpers ------------------------------------

    // Append a vertex to verts. If colorOpt has value -> use it; else derive color from normalized position.
    inline void appendVertex(std::vector<Vertex> &verts,
                             float x, float y, float z,
                             const std::optional<std::array<float, 3>> &colorOpt)
    {
        Vertex v;
        v.x = x;
        v.y = y;
        v.z = z;
        if (colorOpt)
        {
            v.r = (*colorOpt)[0];
            v.g = (*colorOpt)[1];
            v.b = (*colorOpt)[2];
        }
        else
        {
            // color by normalized position for quick visual debugging (maps [-r,r] -> [0,1])
            float r = std::sqrt(x * x + y * y + z * z);
            float nx = (r > 0.0f) ? x / r : 0.0f;
            float ny = (r > 0.0f) ? y / r : 0.0f;
            float nz = (r > 0.0f) ? z / r : 0.0f;
            v.r = 0.5f * (nx + 1.0f);
            v.g = 0.5f * (ny + 1.0f);
            v.b = 0.5f * (nz + 1.0f);
        }
        verts.push_back(v);
    }

    // Add two triangles (quad) using base index
    inline void pushQuadAsTwoTriangles(std::vector<unsigned int> &indices, unsigned int base)
    {
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);

        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 0);
    }

    // Reusable quad face generator: positions in CCW order looking from outside
    inline void addQuadFace(std::vector<Vertex> &outVerts,
                            std::vector<unsigned int> &outIndices,
                            const std::array<std::array<float, 3>, 4> &positions,
                            const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        unsigned int base = static_cast<unsigned int>(outVerts.size());
        for (int i = 0; i < 4; ++i)
        {
            appendVertex(outVerts, positions[i][0], positions[i][1], positions[i][2], colorOpt);
        }
        pushQuadAsTwoTriangles(outIndices, base);
    }

    // ---------------------- Primitive factories (simple) -----------------------

    // Triangle primitive (2D-like but z=0)
    inline std::pair<std::vector<Vertex>, std::vector<unsigned int>> makeTriangleVertsIndices(
        const std::array<std::array<float, 3>, 3> &positions,
        const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        for (int i = 0; i < 3; ++i)
            appendVertex(verts, positions[i][0], positions[i][1], positions[i][2], colorOpt);
        indices = {0, 1, 2};
        return {std::move(verts), std::move(indices)};
    }

    // Quad primitive (four positions, CCW)
    inline std::pair<std::vector<Vertex>, std::vector<unsigned int>> makeQuadVertsIndices(
        const std::array<std::array<float, 3>, 4> &positions,
        const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        for (int i = 0; i < 4; ++i)
            appendVertex(verts, positions[i][0], positions[i][1], positions[i][2], colorOpt);
        pushQuadAsTwoTriangles(indices, 0);
        return {std::move(verts), std::move(indices)};
    }

    // ---------------------- Public convenience creators -----------------------

    // Simple triangle (z = 0)
    inline Mesh CreateTriangle()
    {
        auto [verts, indices] = makeTriangleVertsIndices({std::array<float, 3>{-0.6f, -0.5f, 0.0f},
                                                          std::array<float, 3>{0.6f, -0.5f, 0.0f},
                                                          std::array<float, 3>{0.0f, 0.6f, 0.0f}},
                                                         std::nullopt /* color by position */);

        Mesh m;
        m.initialize(verts, indices);
        return m;
    }

    // Quad (two triangles)
    inline Mesh CreateQuad()
    {
        auto [verts, indices] = makeQuadVertsIndices({std::array<float, 3>{-0.5f, -0.5f, 0.0f},
                                                      std::array<float, 3>{0.5f, -0.5f, 0.0f},
                                                      std::array<float, 3>{0.5f, 0.5f, 0.0f},
                                                      std::array<float, 3>{-0.5f, 0.5f, 0.0f}},
                                                     std::nullopt);

        Mesh m;
        m.initialize(verts, indices);
        return m;
    }

    // Cube built by reusing addQuadFace (keeps 4 verts per face -> 24 verts)
    inline Mesh CreateCube(float size = 1.0f,
                           const std::optional<std::array<float, 3>> &faceColorOpt = std::nullopt)
    {
        float h = size * 0.5f;
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;

        // Face colors fallback (if user didn't pass one, each face will have position-based color)
        const std::array<std::array<float, 3>, 6> defaultFaceColors = {{
            {1.0f, 0.0f, 0.0f}, // front
            {0.0f, 1.0f, 0.0f}, // back
            {0.0f, 0.0f, 1.0f}, // left
            {1.0f, 1.0f, 0.0f}, // right
            {1.0f, 0.0f, 1.0f}, // top
            {0.0f, 1.0f, 1.0f}  // bottom
        }};

        // Helper to choose color: if faceColorOpt has, use it; else use that face's default
        auto faceColorOr = [&](int faceIdx) -> std::optional<std::array<float, 3>>
        {
            if (faceColorOpt)
                return faceColorOpt;
            return defaultFaceColors[faceIdx];
        };

        // front (z = +h)
        addQuadFace(verts, indices, {{{{-h, -h, h}}, {{h, -h, h}}, {{h, h, h}}, {{-h, h, h}}}}, faceColorOr(0));

        // back (z = -h) - winding chosen to face outward
        addQuadFace(verts, indices, {{{{h, -h, -h}}, {{-h, -h, -h}}, {{-h, h, -h}}, {{h, h, -h}}}}, faceColorOr(1));

        // left (x = -h)
        addQuadFace(verts, indices, {{{{-h, -h, -h}}, {{-h, -h, h}}, {{-h, h, h}}, {{-h, h, -h}}}}, faceColorOr(2));

        // right (x = +h)
        addQuadFace(verts, indices, {{{{h, -h, h}}, {{h, -h, -h}}, {{h, h, -h}}, {{h, h, h}}}}, faceColorOr(3));

        // top (y = +h)
        addQuadFace(verts, indices, {{{{-h, h, h}}, {{h, h, h}}, {{h, h, -h}}, {{-h, h, -h}}}}, faceColorOr(4));

        // bottom (y = -h)
        addQuadFace(verts, indices, {{{{-h, -h, -h}}, {{h, -h, -h}}, {{h, -h, h}}, {{-h, -h, h}}}}, faceColorOr(5));

        Mesh m;
        m.initialize(verts, indices);
        return m;
    }

    // ------------------- Spherical patch generator (core reuse) -----------------
    // Generates a rectangular grid in (phi,theta) parameter space, turning into
    // triangle indices. phi range is [phiStart, phiEnd] in radians (0 = +Y pole, PI = -Y pole).
    //
    // Returns pair<verts, indices>
    inline std::pair<std::vector<Vertex>, std::vector<unsigned int>>
    generateSphericalPatch(float radius,
                           unsigned int sectors,
                           unsigned int stacks,
                           float phiStart,
                           float phiEnd,
                           float centerYOffset = 0.0f,
                           const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        if (sectors < 3)
            sectors = 3;
        if (stacks < 1)
            stacks = 1;

        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;

        // vertices: stacks+1 rows, sectors+1 cols (duplicate last column for seam)
        for (unsigned int i = 0; i <= stacks; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(stacks);
            float phi = phiStart + (phiEnd - phiStart) * t; // phi in [phiStart, phiEnd]
            float y = centerYOffset + radius * std::cos(phi);
            float r_xy = radius * std::sin(phi);
            for (unsigned int j = 0; j <= sectors; ++j)
            {
                float s = static_cast<float>(j) / static_cast<float>(sectors);
                float theta = s * 2.0f * PI_F;
                float x = r_xy * std::cos(theta);
                float z = r_xy * std::sin(theta);
                appendVertex(verts, x, y, z, colorOpt);
            }
        }

        const unsigned int rowSize = sectors + 1;
        for (unsigned int i = 0; i < stacks; ++i)
        {
            for (unsigned int j = 0; j < sectors; ++j)
            {
                unsigned int first = i * rowSize + j;
                unsigned int second = first + rowSize;
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(first + 1);
                indices.push_back(second);
                indices.push_back(second + 1);
            }
        }

        return {std::move(verts), std::move(indices)};
    }

    // --- Cylinder generator ----------------------------------------------------
    //
    // radius: radius of cylinder
    // height: full cylinder height (distance between top and bottom cap planes)
    // sectors: around (>=3)
    // stacks: subdivisions along height (>=1). If stacks==1 => single quad ring.
    // caps: whether to add top/bottom disc caps (true/false). Caps generated as fans.
    inline std::pair<std::vector<Vertex>, std::vector<unsigned int>>
    generateCylindricalPatch(float radius,
                             float height,
                             unsigned int sectors,
                             unsigned int stacks,
                             bool caps = true,
                             const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        if (sectors < 3)
            sectors = 3;
        if (stacks < 1)
            stacks = 1;

        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;

        const float halfH = 0.5f * height;

        // Side vertices: stacks+1 rows from -halfH to +halfH, sectors+1 cols
        for (unsigned int i = 0; i <= stacks; ++i)
        {
            float t = static_cast<float>(i) / static_cast<float>(stacks);
            float y = -halfH + t * height;
            for (unsigned int j = 0; j <= sectors; ++j)
            {
                float s = static_cast<float>(j) / static_cast<float>(sectors);
                float theta = s * 2.0f * PI_F;
                float x = radius * std::cos(theta);
                float z = radius * std::sin(theta);
                appendVertex(verts, x, y, z, colorOpt);
            }
        }

        const unsigned int rowSize = sectors + 1;
        // side indices
        for (unsigned int i = 0; i < stacks; ++i)
        {
            for (unsigned int j = 0; j < sectors; ++j)
            {
                unsigned int first = i * rowSize + j;
                unsigned int second = first + rowSize;
                indices.push_back(first);
                indices.push_back(second);
                indices.push_back(first + 1);

                indices.push_back(first + 1);
                indices.push_back(second);
                indices.push_back(second + 1);
            }
        }

        // Caps (optional) — triangle fans using the top/bottom ring vertices
        if (caps)
        {
            // top center
            unsigned int topCenterIndex = static_cast<unsigned int>(verts.size());
            appendVertex(verts, 0.0f, +halfH, 0.0f, colorOpt);

            // top ring start indexes (top row)
            unsigned int topRowStart = stacks * rowSize; // row index for y = +halfH
            // create fan: (topCenter, ring[j+1], ring[j]) to keep outward normal +Y
            for (unsigned int j = 0; j < sectors; ++j)
            {
                unsigned int a = topRowStart + j;
                unsigned int b = topRowStart + j + 1;
                indices.push_back(topCenterIndex);
                indices.push_back(b);
                indices.push_back(a);
            }

            // bottom center
            unsigned int bottomCenterIndex = static_cast<unsigned int>(verts.size());
            appendVertex(verts, 0.0f, -halfH, 0.0f, colorOpt);

            // bottom ring start (bottom row)
            unsigned int bottomRowStart = 0; // first row is bottom y = -halfH
            // bottom fan: outward normal -Y -> triangles should face downward. Use (bottomCenter, ring[j], ring[j+1])
            for (unsigned int j = 0; j < sectors; ++j)
            {
                unsigned int a = bottomRowStart + j;
                unsigned int b = bottomRowStart + j + 1;
                indices.push_back(bottomCenterIndex);
                indices.push_back(a);
                indices.push_back(b);
            }
        }

        return {std::move(verts), std::move(indices)};
    }

    inline Mesh CreateCylinder(float radius = 0.5f,
                               float height = 1.0f,
                               unsigned int sectors = 36,
                               unsigned int stacks = 1,
                               bool caps = true,
                               const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        auto [verts, indices] = generateCylindricalPatch(radius, height, sectors, stacks, caps, colorOpt);
        Mesh m;
        m.initialize(verts, indices);
        return m;
    }

    // Create upper hemisphere (phi: 0..PI/2)
    inline Mesh CreateHemisphere(float radius = 1.0f,
                                 unsigned int sectors = 36,
                                 unsigned int stacks = 18,
                                 const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        auto [verts, indices] = generateSphericalPatch(radius, sectors, stacks, 0.0f, 0.5f * PI_F, 0.0f,colorOpt);
        Mesh m;
        m.initialize(verts, indices);
        return m;
    }

    // Create full sphere (phi: 0..PI)
    inline Mesh CreateSphere(float radius = 1.0f,
                             unsigned int sectors = 36,
                             unsigned int stacks = 18,
                             const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        auto [verts, indices] = generateSphericalPatch(radius, sectors, stacks, 0.0f, PI_F, 0.0f,colorOpt);
        Mesh m;
        m.initialize(verts, indices);
        return m;
    }

    // --- Capsule: compose two hemispheres and a cylinder -----------------------
    // cylinderHeight = length of cylinder part between the equators of hemispheres
    // total capsule height = cylinderHeight + 2*radius
    inline Mesh CreateCapsule(float radius = 0.5f,
                              float cylinderHeight = 1.0f,
                              unsigned int sectors = 36,
                              unsigned int stacksHemisphere = 12,
                              unsigned int stacksCylinder = 1,
                              const std::optional<std::array<float, 3>> &colorOpt = std::nullopt)
    {
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;

        // 1) cylinder (no caps) centered at origin, height = cylinderHeight
        auto [cylVerts, cylIndices] = generateCylindricalPatch(radius, cylinderHeight, sectors, stacksCylinder, false, colorOpt);

        // append cylinder
        unsigned int base = 0;
        verts.insert(verts.end(), cylVerts.begin(), cylVerts.end());
        indices.insert(indices.end(), cylIndices.begin(), cylIndices.end());
        base = static_cast<unsigned int>(cylVerts.size()); // next base offset

        // 2) top hemisphere: phi [0, PI/2], centerY = +cylinderHeight/2
        float topCenterY = +0.5f * cylinderHeight;
        auto [topVerts, topIndices] = generateSphericalPatch(radius, sectors, stacksHemisphere, 0.0f, 0.5f * PI_F, topCenterY, colorOpt);

        // append top hemisphere with index offset
        for (auto &v : topVerts)
            verts.push_back(v);
        for (auto idx : topIndices)
            indices.push_back(static_cast<unsigned int>(idx + base));
        base += static_cast<unsigned int>(topVerts.size());

        // 3) bottom hemisphere: phi [PI/2, PI], centerY = -cylinderHeight/2
        float bottomCenterY = -0.5f * cylinderHeight;
        auto [botVerts, botIndices] = generateSphericalPatch(radius, sectors, stacksHemisphere, 0.5f * PI_F, PI_F, bottomCenterY, colorOpt);

        // append bottom hemisphere with index offset
        for (auto &v : botVerts)
            verts.push_back(v);
        for (auto idx : botIndices)
            indices.push_back(static_cast<unsigned int>(idx + base));
        base += static_cast<unsigned int>(botVerts.size());

        // note: this approach duplicates the equator ring vertices (cylinder top/bottom and hemisphere equators).
        // it's simpler and robust; if you want to share vertices, we can add a special-case merge.

        Mesh m;
        m.initialize(verts, indices);
        return m;
    }

} // namespace MeshFactory
