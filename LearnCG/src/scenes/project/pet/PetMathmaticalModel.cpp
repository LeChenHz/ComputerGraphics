#include "PetMathmaticalModel.h"

PetMathmaticalModel::PetMathmaticalModel()
{
	BDM_number = 12;
	radius = 10.48f / 2.0f;
	DU_number_perBDM = 4;
	Crystall_number_perDU = 13;

	glm::vec3 v_pos;
	float x, y = 0.0f, z;
	float angle_segment = TWO_PI / BDM_number;
	for (int i = 0; i <= BDM_number; i++)
	{
		float angle = angle_segment * i;
		x = radius * cosf(angle);
		z = radius * sinf(angle);
		v_pos = glm::vec3(x, y, z);
		vertexs_pos.push_back(v_pos);
	}
	BDM_width = glm::distance(vertexs_pos[0], vertexs_pos[1]);
	BDM_length = DU_number_perBDM * BDM_width;

	//对每个探测器进行建模
	glm::vec3 y_offset = up_normal_vec * BDM_length / 2.0f;
	for (int i = 0; i < BDM_number; i++)
	{
		BDM_model bdm;
		bdm.center = (vertexs_pos[i] + vertexs_pos[i + 1]) / 2.0f;
		bdm.right_normal_vec = glm::normalize(vertexs_pos[i] - vertexs_pos[i + 1]);
		glm::vec3 xz_offset = bdm.right_normal_vec * BDM_width / 2.0f;
		bdm.v1 = bdm.center - xz_offset + y_offset;
		bdm.v2 = bdm.center + xz_offset + y_offset;
		bdm.v3 = bdm.center + xz_offset - y_offset;
		bdm.v4 = bdm.center - xz_offset - y_offset;
		BDMs.push_back(bdm);
	}

	// 对探测器里的探头进行建模
	for (int i = 0; i < BDM_number; i++)
	{
		DU_model du;
		du.v1 = BDMs[i].v1;
		du.v2 = BDMs[i].v2;
		glm::vec3 bdm_v1tov4 = (BDMs[i].v4 - BDMs[i].v1) / glm::vec3(DU_number_perBDM);
		glm::vec3 bdm_v2tov3 = (BDMs[i].v3 - BDMs[i].v2) / glm::vec3(DU_number_perBDM);
		du.v3 = du.v2 + bdm_v2tov3;
		du.v4 = du.v1 + bdm_v1tov4;
		BDMs[i].DUs.push_back(du);
		for (int j = 1; j < DU_number_perBDM; j++)
		{
			du.v1 += bdm_v1tov4;
			du.v2 += bdm_v2tov3;
			du.v3 += bdm_v2tov3;
			du.v4 += bdm_v1tov4;
			BDMs[i].DUs.push_back(du);
		}
	}

	// 对探头里的晶格进行建模
	Crystal_model crystal;
	for (int i = 0; i < BDM_number; i++)
	{
		for (int j = 0; j < DU_number_perBDM; j++)
		{
			glm::vec3 du_v1 = BDMs[i].DUs[j].v1;
			glm::vec3 du_v1tov2 = (BDMs[i].DUs[j].v2 - BDMs[i].DUs[j].v1) / glm::vec3(Crystall_number_perDU);
			glm::vec3 du_v1tov4 = (BDMs[i].DUs[j].v4 - BDMs[i].DUs[j].v1) / glm::vec3(Crystall_number_perDU);
			du_v1 += (du_v1tov2 + du_v1tov4) / glm::vec3(2.0f); //晶格1
			for (int k = 0; k < Crystall_number_perDU * Crystall_number_perDU; k++)
			{
				crystal.center = du_v1 
					+ du_v1tov2 * glm::vec3(k % Crystall_number_perDU) 
					+ du_v1tov4 * glm::vec3(k / Crystall_number_perDU);
				BDMs[i].DUs[j].CRYSTALs.push_back(crystal);
			}
		}
	}

	init();
}

PetMathmaticalModel::~PetMathmaticalModel()
{
	glDeleteVertexArrays(1, &BDM_VAO);
	glDeleteBuffers(1, &BDM_VBO);
	glDeleteVertexArrays(1, &DU_VAO);
	glDeleteBuffers(1, &DU_VBO);
	delete[] g_bdm_position_data;
	delete[] g_du_position_data;
}

void PetMathmaticalModel::init()
{
	//将探测器顶点数据填充到数组
	g_bdm_position_data = new GLfloat[BDM_number * 6 * 3];
	for (int i = 0; i < BDM_number; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			glm::vec3 v;
			if (j == 0) v = BDMs[i].v1;
			else if (j == 1) v = BDMs[i].v2;
			else if (j == 2) v = BDMs[i].v3;
			else if (j == 3) v = BDMs[i].v1;
			else if (j == 4) v = BDMs[i].v3;
			else if (j == 5) v = BDMs[i].v4;
			g_bdm_position_data[i * 6 * 3 + j * 3 + 0] = v.x;
			g_bdm_position_data[i * 6 * 3 + j * 3 + 1] = v.y;
			g_bdm_position_data[i * 6 * 3 + j * 3 + 2] = v.z;
		}
	}

	//将探头顶点数据填充到数组
	g_du_position_data = new GLfloat[BDM_number * 6 * 3 * DU_number_perBDM];
	for (int i = 0; i < BDM_number; i++)
	{
		for (int j = 0; j < DU_number_perBDM; j++)
		{
			for (int k = 0; k < 6; k++)
			{
				glm::vec3 v;
				if (k == 0) v = BDMs[i].DUs[j].v1;
				else if (k == 1) v = BDMs[i].DUs[j].v2;
				else if (k == 2) v = BDMs[i].DUs[j].v3;
				else if (k == 3) v = BDMs[i].DUs[j].v1;
				else if (k == 4) v = BDMs[i].DUs[j].v3;
				else if (k == 5) v = BDMs[i].DUs[j].v4;
				g_du_position_data[i * DU_number_perBDM * 6 * 3 + j * 6 * 3 + k * 3 + 0] = v.x;
				g_du_position_data[i * DU_number_perBDM * 6 * 3 + j * 6 * 3 + k * 3 + 1] = v.y;
				g_du_position_data[i * DU_number_perBDM * 6 * 3 + j * 6 * 3 + k * 3 + 2] = v.z;
			}
		}
	}


	glGenVertexArrays(1, &BDM_VAO);
	glGenBuffers(1, &BDM_VBO);
	glBindVertexArray(BDM_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, BDM_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BDM_number * 6 * 3, g_bdm_position_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &DU_VAO);
	glGenBuffers(1, &DU_VBO);
	glBindVertexArray(DU_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, DU_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * BDM_number * DU_number_perBDM * 6 * 3, g_du_position_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void PetMathmaticalModel::draw(Shader *shader)
{
	shader->use();
	if (true)
	{
		shader->setVec3("color", 161.0f / 255, 161.0f / 255, 123.0f / 255);
		glBindVertexArray(BDM_VAO);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, BDM_number * 6 * 3);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (true)
	{
		shader->setVec3("color", 255.0f / 255, 255.0f / 255, 255.0f / 255);
		glBindVertexArray(BDM_VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, BDM_number * 6 * 3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (false)
	{
		shader->setVec3("color", 255.0f / 255, 255.0f / 255, 255.0f / 255);
		glBindVertexArray(DU_VAO);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, BDM_number * DU_number_perBDM * 6 * 3);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

}
