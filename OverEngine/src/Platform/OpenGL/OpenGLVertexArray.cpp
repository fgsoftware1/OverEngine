#include "pcheader.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace OverEngine
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case OverEngine::ShaderDataType::Float:    return GL_FLOAT;
		case OverEngine::ShaderDataType::Float2:   return GL_FLOAT;
		case OverEngine::ShaderDataType::Float3:   return GL_FLOAT;
		case OverEngine::ShaderDataType::Float4:   return GL_FLOAT;
		case OverEngine::ShaderDataType::Mat3:     return GL_FLOAT;
		case OverEngine::ShaderDataType::Mat4:     return GL_FLOAT;
		case OverEngine::ShaderDataType::Int:      return GL_INT;
		case OverEngine::ShaderDataType::Int2:     return GL_INT;
		case OverEngine::ShaderDataType::Int3:     return GL_INT;
		case OverEngine::ShaderDataType::Int4:     return GL_INT;
		case OverEngine::ShaderDataType::Bool:     return GL_BOOL;
		}

		OE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		OE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}