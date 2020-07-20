#include "MyApp.h"
#include "GLUtils.hpp"

#include <GL/GLU.h>
#include <math.h>

#include "ObjParser_OGL3.h"

CMyApp::CMyApp(void)
{
	dirY = dirZ = dirX = 0.0;
	m_textureID = 0;
	m_textureID2 = 0;
	m_mesh = 0;
	
	
}


CMyApp::~CMyApp(void)
{
}


GLuint CMyApp::GenTexture()
{
	unsigned char tex[256][256][3];

	for (int i = 0; i<256; ++i)
	for (int j = 0; j<256; ++j)
	{
		tex[i][j][0] = rand() % 256;
		tex[i][j][1] = rand() % 256;
		tex[i][j][2] = rand() % 256;
	}

	GLuint tmpID;

	glGenTextures(1, &tmpID);
	glBindTexture(GL_TEXTURE_2D, tmpID);
	gluBuild2DMipmaps(GL_TEXTURE_2D,
		GL_RGB8, 256, 256,			// define storage on the GPU: RGB, 8bits per channel, 256x256 texture
		GL_RGB, GL_UNSIGNED_BYTE,	// define storate in RAM: RGB layout, unsigned bytes for each channel
		tex);						//						  and source pointer
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// bilinear filter on min and mag
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tmpID;
}

void CMyApp::SurfaceEquation(float u, float v, glm::vec3& point, glm::vec3& normal, glm::vec2& texcoord) {
	float r = 2;
	u *= 2*3.1415f;
	v *= 3.1415f;
	float cu = cosf(u), su = sinf(u), cv = cosf(v), sv = sinf(v);
	
	point = glm::vec3(r*cu*sv, r*cv, r*su*sv);
	normal = glm::vec3(cu*sv, cv, su*sv);
	texcoord = glm::vec2(u, v);
}

void CMyApp::genParametricSurface(int n, gVertexBuffer &buffer) {
	for(int i=0;i<n;++i) for(int j=0;j<n;++j)
	{
		float u = i/(float)n, u_ = (i+1)/(float)n, v = j/(float)n, v_ = (j+1)/(float)n;
		glm::vec3 P1, P2, P3, P4, N1, N2, N3, N4;
		glm::vec2 T1, T2, T3, T4;
		
		SurfaceEquation(u, v,   P1, N1, T1);
		SurfaceEquation(u_, v,  P2, N2, T2);
		SurfaceEquation(u, v_,  P3, N3, T3);
		SurfaceEquation(u_, v_, P4, N4, T4);
		
		buffer.AddData(0, P1); buffer.AddData(0, P2); buffer.AddData(0, P3);
		
		buffer.AddData(0, P3); buffer.AddData(0, P2); buffer.AddData(0, P4);
		
		buffer.AddData(1, N1); buffer.AddData(1, N2); buffer.AddData(1, N3);
		
		buffer.AddData(1, N3); buffer.AddData(1, N2); buffer.AddData(1, N4);
		
		buffer.AddData(2, T1); buffer.AddData(2, T2); buffer.AddData(2, T3);
		
		buffer.AddData(2, T3); buffer.AddData(2, T2); buffer.AddData(2, T4);
		
		buffer.AddIndex((i*n+j)*6+0, (i*n+j)*6+1, (i*n+j)*6+2);
		buffer.AddIndex((i*n+j)*6+3, (i*n+j)*6+4, (i*n+j)*6+5);
	}
}


void CMyApp::add_triangle(
	const glm::vec3 &P1,
	const glm::vec3 &P2,
	const glm::vec3 &P3,
	gVertexBuffer &buffer) {

	buffer.AddData(0, P1.x, P1.y, P1.z); // P1
	buffer.AddData(0, P2.x, P2.y, P2.z); // P2
	buffer.AddData(0, P3.x, P3.y, P3.z); // P3

	glm::vec3 V1 = P2 - P1; //P2-P1
	glm::vec3 V2 = P3 - P1; //P3-P1
	glm::vec3 normal = glm::normalize(glm::cross(V1, V2));
	
	//normals
	buffer.AddData(1, normal.x, normal.y, normal.z);
	buffer.AddData(1, normal.x, normal.y, normal.z);
	buffer.AddData(1, normal.x, normal.y, normal.z);

	//texture coordinates
	buffer.AddData(2, 0, 0);
	buffer.AddData(2, 0.5, 1);
	buffer.AddData(2, 1, 0);
}

bool CMyApp::Init()
{
	// background color
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	// set cullface and z-buffer ON, cull backfaces
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);

	//
	// create the geometry
	//

	m_vb.AddAttribute(0, 3); //positions
	m_vb.AddAttribute(1, 3); //normals 
	m_vb.AddAttribute(2, 2); // tex coords

	//plane of dimensions 30x30
	//positions
	m_vb.AddData(0, -25, 0, -25);
	m_vb.AddData(0, 25, 0, -25);
	m_vb.AddData(0, -25, 0, 25);
	m_vb.AddData(0, 25, -0, 25);

	// normals
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);
	m_vb.AddData(1, 0, 1, 0);

	// tex coords
	m_vb.AddData(2, 0, 0);
	m_vb.AddData(2, 1, 0);
	m_vb.AddData(2, 0, 1);
	m_vb.AddData(2, 1, 1);

	m_vb.AddIndex(1, 0, 2);
	m_vb.AddIndex(1, 2, 3);

	m_vb.InitBuffers();

	

	m_vb_pyram.AddAttribute(0, 3); //positions
	m_vb_pyram.AddAttribute(1, 3); //normals 
	m_vb_pyram.AddAttribute(2, 2); // tex coords


	//construction of pyriamid
	//adding triangles based on dimensions
	//!!! be careful about total size !!!
	//demo : (GL_TRIANGLES, 0, *30*, 0); //IB
	//double mx = 0.5;
	//double mz = sqrt(3.0) / 2.0;
	//double n = 1;
	//double h = 3.0;

	double r = 1.0;
	double a = r * sqrt(2.0);
	double h = sqrt(a * a - 0.5*a);

/**/
	//4 side s of base
	// order : soldan saga
	add_triangle(
		glm::vec3(r, 0, 0),
		glm::vec3(0, 0, r),
		glm::vec3(0, 0, 0),
		m_vb_pyram);
	m_vb_pyram.AddIndex(0, 1, 2);


	add_triangle(
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, r),
		glm::vec3(-r, 0, 0),
		m_vb_pyram);
	m_vb_pyram.AddIndex(3, 4, 5);


	add_triangle(
		glm::vec3(0, 0, 0),
		glm::vec3(-r, 0, 0),
		glm::vec3(0, 0, -r),
		m_vb_pyram);
	m_vb_pyram.AddIndex(6, 7, 8);

	add_triangle(
		glm::vec3(0, 0, -r),
		glm::vec3(r, 0, 0),
		glm::vec3(0, 0, 0),
		
		m_vb_pyram);	
	m_vb_pyram.AddIndex(9, 10, 11);

	//-----------------------------------------------------------------------sides
	
	add_triangle(
		glm::vec3(0, 0, r),
		glm::vec3(r, 0, 0),
		glm::vec3(0, h, 0),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(12, 13, 14);

	
	add_triangle(
		glm::vec3(0, 0, r),
		glm::vec3(0, h, 0),
		glm::vec3(-r, 0, 0),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(15, 16, 17);
	

	add_triangle(
		glm::vec3(-r, 0, 0),
		glm::vec3(0, h, 0),
		glm::vec3(0, 0, -r),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(18, 19, 20);
	
	add_triangle(
		glm::vec3(r, 0, 0),
		glm::vec3(0, 0, -r),
		glm::vec3(0, h, 0),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(21, 22, 23);

	//-----------------------------------------------------------------------under sides
	add_triangle(
		glm::vec3(r, 0, 0),
		glm::vec3(0, 0, r),
		glm::vec3(0, -h, 0),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(24, 25, 26);


	add_triangle(
		glm::vec3(0, -h, 0),
		glm::vec3(0, 0, r),
		glm::vec3(-r, 0, 0),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(27, 28, 29);


	add_triangle(
		glm::vec3(0, -h, 0),
		glm::vec3(-r, 0, 0),
		glm::vec3(0, 0, -r),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(30, 31, 32);

	add_triangle(
		glm::vec3(0, 0, -r),
		glm::vec3(r, 0, 0),
		glm::vec3(0, -h, 0),
		m_vb_pyram
	);
	m_vb_pyram.AddIndex(33, 34, 35);
	
	m_vb_pyram.InitBuffers(); // IMPORTANT!!!! moves vertices to GPU

	//
	// load shaders	
	//
	m_program.AttachShader(GL_VERTEX_SHADER, "dirLight.vert");
	m_program.AttachShader(GL_FRAGMENT_SHADER, "dirLight.frag");

	m_program.BindAttribLoc(0, "vs_in_pos");
	m_program.BindAttribLoc(1, "vs_in_normal");
	m_program.BindAttribLoc(2, "vs_in_tex0");

	if ( !m_program.LinkProgram() )
	{
		return false;
	}

	//
	// misc init
	//

	m_camera.SetProj(45.0f, 640.0f/480.0f, 0.01f, 1000.0f);

	m_textureID = TextureFromFile("fish.jpg");
	m_textureID2 = TextureFromFile("water.bmp");

	m_mesh = ObjParser::parse("fish.obj");
	m_mesh->initBuffers();
	
	//25 + (std::rand() % (63 - 25 + 1))

	for (int i = 0; i < 10; i++) {
		randx = -15 + (std::rand() % (15 - (-15) + 1));
		randz = -15 + (std::rand() % (15 - (-15) + 1));
		randposition.push_back(glm::vec3(randx, 0, randz));
	}
	return true;
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_textureID);

	m_program.Clean();
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time)/1000.0f;

	m_camera.Update(delta_time);

	last_time = SDL_GetTicks();
}

void CMyApp::Render()
{
	elapsedTime = SDL_GetTicks() / 1000.0f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// draw ground ----------------------------------------------------------------

	m_program.On();


	glm::mat4 matWorld = glm::mat4(1.0f);
	glm::mat4 matWorldIT = glm::transpose(glm::inverse(matWorld));
	glm::mat4 mvp = m_camera.GetViewProj() * matWorld;

	m_program.SetUniform("world", matWorld);
	m_program.SetUniform("worldIT", matWorldIT);
	m_program.SetUniform("MVP", mvp);
	m_program.SetUniform("eye_pos", m_camera.GetEye());
	int isHex = 1;
	//m_program.SetUniform("isHex", isHex);
	m_program.SetTexture("texImage", 0, m_textureID2);

	// draw with VAO
	m_vb.On();

	m_vb.DrawIndexed(GL_TRIANGLES, 0, 6, 0); //IB

	m_vb.Off();

	isHex = 0;
//--------------------------------------------------------------------draw pyramid	
	int t = SDL_GetTicks();  //int because its fast, spins around y axis,
	                         //karusel kimi firlatmaq, globus kimi
	float time_hex = SDL_GetTicks() / 200.0;;
	
	double time = SDL_GetTicks() / 5000.0f; // 5 seconds interval goes down
	double x = 5.0 * cosf(time);


	uint32_t last_tick_time = 0;
	uint32_t delta = 0;

	
	
	//deltaTime = elapsedTime - lastFrameTimeElapsed;
	
	
	for (int i = 0; i < 5; i++) {
		double angle = 2 * M_PI / 5.0 * i + 2 * M_PI * t / 5000.0; //10 dene esya ucun yerlemse bucagi
		double x = 5 * cosf(angle); //1 fiqurun x coordinati mustevi uzerinde 
		double z = 5 * sinf(angle); //1 fiquruny coordinati mustevi uzerinde 
		//double y = abs(20.0 * sinf(time)); //y coordinatinin deyismesi yuxari 
		//std::cout << y << " ";
		m_program.SetUniform("light_pos", glm::vec3(x, 10, z));

		matWorld = glm::translate(glm::vec3(x, 20, z)); //set starting position for hexagonpyramid
		//matWorld = glm::translate(glm::vec3(dirX + x, 20.0 - y, dirZ + z));
		//transform pyramid positions (translate rotate)

		  
			matWorld = glm::translate(glm::vec3(dirX + x, 0, dirZ + z)) * glm::rotate<float>(M_PI, glm::vec3(1, 0, 0)) //basi asagi rotate
				*
				glm::rotate<float>(M_PI + time_hex, glm::vec3(0, 1, 0)) * glm::mat4(1.0f); //rotating around y axis (itself)
        
			//uint32_t tick_time = SDL_GetTicks();

			//if(time < 5.0)
			//delta = tick_time - last_tick_time;
			//last_tick_time = tick_time;

		//std::cout << t << " " <<std::endl;
			

		matWorldIT = glm::transpose(glm::inverse(matWorld));
		mvp = m_camera.GetViewProj() * matWorld;

		m_program.SetUniform("world", matWorld);
		m_program.SetUniform("worldIT", matWorldIT);
		m_program.SetUniform("MVP", mvp);
		m_program.SetUniform("eye_pos", m_camera.GetEye());
		isHex = 1;
		//std::cout << isHex;
		m_program.SetUniform("isHex", isHex);
		//m_program.SetTexture("texImage", 0, m_textureID);

		// draw with VAO
		m_vb_pyram.On();
		m_vb_pyram.Draw(GL_TRIANGLES, 0, 36); //IB
		m_vb_pyram.Off();
	}
 //-------------------------------------------------------------------------------------------------------
	
	// draw mesh
    // draw xmas tree on the top of the ground //touches the ground in center
	for (int i = 0; i < 10; i++) {
		
		double anglef = 2 * M_PI / 10.0 * i + 2 * M_PI * t / 5000.0; //10 dene esya ucun yerlemse bucagi
		double xf = 5 * cosf(anglef); //1 fiqurun x coordinati mustevi uzerinde 
		double zf = 5 * sinf(anglef); //1 fiquruny coordinati mustevi uzerinde
		float piangle = 0.0;
		glm::mat4 move = glm::translate(glm::vec3(dirX, dirY, dirZ));
		float size = 0.2;
		if (direction == 1) {     //to the north
			if (orig_direction == 1) { ; }

			else if (orig_direction == 2)
				piangle = M_PI / 2.0;

			else if (orig_direction == 3)
				piangle = M_PI;

			else if (orig_direction == 4)
				piangle = -M_PI / 2.0;
		}

		if (direction == 3) {     //to the south
			if (orig_direction == 3) { ; }

			else if (orig_direction == 2)
				piangle = -M_PI / 2.0;

			else if (orig_direction == 1)
				piangle = -M_PI;

			else if (orig_direction == 4)
				piangle = M_PI / 2.0;
		}


		if (direction == 2) {     //to the east
			if (orig_direction == 2) { ; }

			else if (orig_direction == 1)
				piangle = M_PI / 2.0;

			else if (orig_direction == 3)
				piangle = -M_PI / 2.0;

			else if (orig_direction == 4)
				piangle = M_PI;
		}

		if (direction == 4) {     //to the west
			if (orig_direction == 4) { ; }

			else if (orig_direction == 2)
				piangle = -M_PI;

			else if (orig_direction == 3)
				piangle = M_PI / 2.0;

			else if (orig_direction == 1)
				piangle = -M_PI / 2.0;
		}

		randx = randposition[i].x;
		randz = randposition[i].z;
		std::cout << randx << " " << randz <<std::endl;

		//orig_direction = direction;
		matWorld = move * glm::translate<float>(glm::vec3(xf + randx, dirY, zf + randz)) * glm::scale(glm::vec3(size)) * glm::rotate<float>(-M_PI / 2.0, glm::vec3(1, 0, 0))
			* glm::rotate<float>(piangle, glm::vec3(0, 0, 1));  //piangle to change the orientation


		matWorldIT = glm::transpose(glm::inverse(matWorld));
		mvp = m_camera.GetViewProj() * matWorld;

		m_program.SetUniform("world", matWorld);
		m_program.SetUniform("worldIT", matWorldIT);
		m_program.SetUniform("MVP", mvp);
		m_program.SetUniform("eye_pos", m_camera.GetEye());
		isHex = 0;
		m_program.SetUniform("isHex", isHex);
		m_program.SetTexture("texImage", 0, m_textureID);
		m_mesh->draw();
	}

}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardDown(key);

	//movements of mesh xmas tree
	switch (key.keysym.sym) {
	case SDLK_UP:
		dirZ -= 1;
		direction = 1;
		break;
	case SDLK_DOWN:
		dirZ += 1;
		direction = 3;
		break;
	case SDLK_RIGHT:
		dirX += 1;
		direction = 2;
		break;
	case SDLK_LEFT:
		dirX -= 1;
		direction = 4;
		break;
	case SDLK_SPACE:
		pressed = !pressed;
		if (!pressed) {
			dirY += 3;
		}
		else
			dirY -= 3;
		
		break;

	default:
		break;
	}
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	m_camera.KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	m_camera.MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
}

void CMyApp::Resize(int _w, int _h)
{
	glViewport(0, 0, _w, _h);

	m_camera.Resize(_w, _h);
}
