#include "sierpinski.h"

Sierpinski::Sierpinski(int iterationCount)
{
    Vertex v1{0.0f, 1.0f};
    Vertex v2{0.866f, -0.5f};
    Vertex v3{-0.866f, -0.5f};

    add(v1, v2, v3);

    for (int i = 0; i < iterationCount; i++)
    {
        iterate();
    }
}

Sierpinski::Vertex Sierpinski::mid(const Sierpinski::Vertex &v1, const Sierpinski::Vertex &v2)
{
    return Vertex{(v1.x + v2.x) / 2, (v1.y + v2.y) / 2};
}

void Sierpinski::add(const Sierpinski::Vertex &v)
{
    m_vertices.push_back(v.x);
    m_vertices.push_back(v.y);
}

void Sierpinski::add(const Sierpinski::Vertex &v1, const Sierpinski::Vertex &v2, const Sierpinski::Vertex &v3)
{
    add(v1);
    add(v2);
    add(v3);
}

void Sierpinski::iterate()
{
    std::vector<float> cur;
    cur.swap(m_vertices);
    m_vertices.reserve(cur.size() * 3);

    for (int i = 0; i < cur.size(); i += 6)
    {
        Vertex v1{cur[i], cur[i + 1]};
        Vertex v2{cur[i + 2], cur[i + 3]};
        Vertex v3{cur[i + 4], cur[i + 5]};

        Vertex v12 = mid(v1, v2);
        Vertex v23 = mid(v2, v3);
        Vertex v31 = mid(v3, v1);

        add(v1, v12, v31);
        add(v12, v2, v23);
        add(v31, v23, v3);
    }
}
