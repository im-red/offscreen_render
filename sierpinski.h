#ifndef SIERPINSKI_H
#define SIERPINSKI_H

#include <vector>

class Sierpinski
{
public:
    explicit Sierpinski(int iterationCount);

    float *data() { return m_vertices.data(); }
    size_t size() const { return m_vertices.size(); }
    size_t vertexCount() const { return m_vertices.size() / 2; }

private:
    struct Vertex
    {
        float x;
        float y;
    };

private:
    Vertex mid(const Vertex &v1, const Vertex &v2);
    void add(const Vertex &v);
    void add(const Vertex &v1, const Vertex &v2, const Vertex &v3);

private:
    void iterate();

private:
    std::vector<float> m_vertices;
};

#endif // SIERPINSKI_H
