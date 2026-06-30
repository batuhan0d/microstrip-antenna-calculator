#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_opengl3_loader.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include "./include/reqtangular.h"
#define STB_IMAGE_IMPLEMENTATION
#include "./include/stb_image.h"
//functions
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
    int image_width = 0;
    int image_height = 0;
    
    unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
    if (image_data == NULL) return false; 

    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);

    stbi_image_free(image_data);

    *out_texture = image_texture;
    *out_width = image_width;
    *out_height = image_height;

    return true;
}

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "Glfw Error " << error << ": " << description << std::endl;
}
static void mm_textAdd(ImFont* largeFont, ImFont* mediumFont) {
    // Set the font for the unit text
    ImGui::PushFont(largeFont);// set large font
    float largeTextHeight = ImGui::CalcTextSize("A").y;
    ImGui::PopFont();// back to previous font again
    ImGui::PushFont(mediumFont);// set medium font
    float mediumTextHeight = ImGui::CalcTextSize("A").y;
    ImGui::SameLine();
    float currentY = ImGui::GetCursorPosY();
    // Calculate the baseline offset
    ImGui::SetCursorPosY(currentY + (largeTextHeight - mediumTextHeight) * 0.80f); // Adjust Y 
    ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "mm");
    ImGui::PopFont();// back to previous font 
}
// Structure to hold image data
struct AppImage {
    GLuint textureID = 0;
    int width = 0;
    int height = 0;
    bool isLoaded = false;
};
int main(){
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit()) return 1;
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // disable window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Patch Antenna calculate", NULL, NULL);
    if (window == NULL) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //font
    io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 14.0f);
    ImFont* largeFont = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 32.0f);
    ImFont* mediumFont = io.Fonts->AddFontFromFileTTF("fonts/Roboto-Medium.ttf", 20.0f);
    //variables
    const float startbox1 = 420.0f;
    const float textsz = 32.0f;
    const float startbox2 = 250.0f;
    const float the_next1 =100.0f;
    const char* hertz[]= {"MHz","GHz"};
    static int hertz_current = 0;
    static float frequency = 0.0;
    static float h = 0.0;
    static float er = 0.0;
    //antenna_out variables
    float width = 0.0;
    float length = 0.0;
    float g_width = 0.0;
    float g_length = 0.0;
    //image variables, and load the image
    AppImage patchSchema;
    patchSchema.isLoaded = LoadTextureFromFile("image/w600antenna.png", &patchSchema.textureID, &patchSchema.width, &patchSchema.height);
    AppImage theBook;
    theBook.isLoaded = LoadTextureFromFile("image/book.jpg", &theBook.textureID, &theBook.width, &theBook.height);
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start the ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Set the ImGui window position to the main viewport's work area position
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        // Set the ImGui window size to the main viewport's work area size
        ImGui::SetNextWindowSize(viewport->WorkSize);
        // ImGui window flags to remove title bar, resizing, moving, etc.
        ImGuiWindowFlags window_flags = 0;
        window_flags |= ImGuiWindowFlags_NoTitleBar; // Disable title-bar
        window_flags |= ImGuiWindowFlags_NoResize; // Disable user resizing with the lower-right grip
        window_flags |= ImGuiWindowFlags_NoMove; // Disable user moving the window
        window_flags |= ImGuiWindowFlags_NoCollapse; // Remove the reduction triangle(disable collapsing the window)
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus; // Keep the window in the background
        window_flags |= ImGuiWindowFlags_NoNavFocus; // Don't let the keyboard distract you

        // Begin the ImGui window with the specified flags
        ImGui::Begin("Antenna calculator", NULL, window_flags);
        
        ImGui::PushFont(mediumFont);// set medium font
        if(ImGui::BeginTabBar("Antenna Tabs", ImGuiTabBarFlags_None))
        {

            if(ImGui::BeginTabItem("Calculate"))
            {

                float panel_width = ImGui::GetWindowWidth() * 0.5f;
                ImGui::BeginChild("Input Parameters", ImVec2(panel_width , 0), true);

                    //start header
                    ImGui::PushFont(largeFont);// set large font
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Input Parameters");
                    ImGui::PopFont();// back to medium font
                    ImGui::Separator();
                    ImGui::Spacing();

                    //first row
                    //ImGui::Dummy(ImVec2(0.0f, 170.0f));
                    ImGui::PushFont(largeFont);// set large font
                    float largeTextHeight = ImGui::CalcTextSize("A").y;
                    ImGui::Text("Frequency:");
                    ImGui::PopFont();// back to medium font
                    //curser position for input box
                    float inputHeight = ImGui::GetFrameHeight();
                    float yOffset = (largeTextHeight - inputHeight) * 0.5f;
                    

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(startbox1);
                    float cursorPosY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cursorPosY + yOffset);
                    ImGui::SetNextItemWidth(the_next1);
                    ImGui::InputFloat("##frequency", &frequency);

                    ImGui::SameLine();
                    ImGui::SetCursorPosY(cursorPosY + yOffset);
                    ImGui::SetNextItemWidth(the_next1);
                    ImGui::Combo("##unit", &hertz_current, hertz, 2);

                    //second row
                    ImGui::PushFont(largeFont);// set large font
                    ImGui::Text("Dielectric substrate height(mm):");
                    ImGui::PopFont();// back to medium font

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(startbox1);
                    cursorPosY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cursorPosY + yOffset);
                    ImGui::SetNextItemWidth(the_next1);
                    ImGui::InputFloat("##height", &h);
                    
                    //third row
                    ImGui::PushFont(largeFont);// set large font
                    ImGui::Text("Relative Permittivity:");
                    ImGui::PopFont();// back to medium font

                    ImGui::SameLine();
                    ImGui::SetCursorPosX(startbox1);
                    cursorPosY = ImGui::GetCursorPosY();
                    ImGui::SetCursorPosY(cursorPosY + yOffset);
                    ImGui::SetNextItemWidth(the_next1);
                    ImGui::InputFloat("##er", &er);

                    //calculate button
                    ImGui::PushFont(largeFont);// set large font
                    ImGui::Dummy(ImVec2(0.0f, 70.0f));
                    float button_width = ImGui::CalcTextSize("Calculate").x + ImGui::GetStyle().FramePadding.x * 2.0f;
                    ImGui::SetCursorPosX((panel_width - button_width) * 0.5f);
                    if(ImGui::Button("Calculate"))
                    {
                        if (frequency <= 0.0f || h <= 0.0f || er <= 0.0f) 
                        {
                            ImGui::OpenPopup("Invalid Input"); 
                        }
                        else
                        {
                            Reqtangular antenna(frequency, hertz_current + 1, er, h);
                            // Convert to mm
                            width = antenna.get_width() * 1000; 
                            length = antenna.get_length() * 1000; 
                            g_width = antenna.get_g_width() * 1000; 
                            g_length = antenna.get_g_length() * 1000; 
                        }
                        
                    }
                    ImGui::PopFont();// back to medium font
                    //style for the popup
                    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.8f, 0.1f, 0.1f, 1.0f)); 
                    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));       
                    ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.2f, 0.05f, 0.05f, 1.0f));

                    ImGui::PushFont(mediumFont);// set medium font for the popup
                    if (ImGui::BeginPopupModal("Invalid Input", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
                    {
                        ImGui::Text("Please fill in all the required fields.");
                        ImGui::Dummy(ImVec2(0.0f, 30.0f));
                        float get_window_width = ImGui::GetWindowWidth();
                        float button_width = 80.0f; // Set a fixed width for the button
                        ImGui::SetCursorPosX((get_window_width - button_width) * 0.5f);
                        // Style the button with a red color    
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));        // Normal state
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); // Hovered state
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.0f, 0.0f, 1.0f)); // Active state
                        if (ImGui::Button("OK", ImVec2(button_width, 0)))
                        {
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::PopStyleColor(3); // Pop the button style colors
                        ImGui::EndPopup();
                    }
                    ImGui::PopFont();// back to medium font again
                    ImGui::PopStyleColor(3); //style colors back to default
                ImGui::EndChild();
                ImGui::SameLine();
                ImGui::BeginChild("Output Parameters", ImVec2(0, 0), true);

                    //start header
                    ImGui::PushFont(largeFont);// set large font
                    ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "Output Parameters");
                    ImGui::PopFont();// back to medium font
                    ImGui::Separator();
                    ImGui::Spacing();

                    if (patchSchema.isLoaded) 
                    {
                        
                        float panel_width = ImGui::GetWindowWidth();
                        float image_width = panel_width * 0.7f; 
                        float image_height = image_width * ((float)patchSchema.height / (float)patchSchema.width); 
                        ImGui::SetCursorPosX((panel_width - image_width) * 0.5f);
                        ImGui::Image((void*)(intptr_t)patchSchema.textureID, ImVec2(image_width, image_height));
                        
                    }
                    else 
                    {
                        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Image could not be loaded!");// text in red color
                        
                    }
                    ImGui::Dummy(ImVec2(0.0f, 30.0f));
                    ImGui::PushFont(largeFont);// set large font
                    ImGui::Text("Patch Width: ");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(startbox2);
                    ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "%.4f", width);
                    mm_textAdd(largeFont, mediumFont);

                    ImGui::Text("Patch Length: ");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(startbox2);
                    ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "%.4f", length);
                    mm_textAdd(largeFont, mediumFont);

                    ImGui::Text("Ground Width: ");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(startbox2);
                    ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "%.4f", g_width);
                    mm_textAdd(largeFont, mediumFont);
                    
                    ImGui::Text("Ground Length: ");
                    ImGui::SameLine();
                    ImGui::SetCursorPosX(startbox2);
                    ImGui::TextColored(ImVec4(0.4f, 0.9f, 0.4f, 1.0f), "%.4f", g_length);
                    mm_textAdd(largeFont, mediumFont);
                    ImGui::PopFont();// back to medium font from large font

                ImGui::EndChild();
                
                ImGui::EndTabItem();
            }
            
            if(ImGui::BeginTabItem("About"))
            {
                

                ImGui::PushFont(largeFont);// set large font
                ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "About");
                ImGui::TextWrapped("This is a simple microstrip patch antenna calculator application built using Dear ImGui and C++. It allows users to input parameters such as frequency, dielectric substrate height, and relative permittivity to calculate antenna characteristics.");
                ImGui::Dummy(ImVec2(0.0f, 15.0f));
                ImGui::TextColored(ImVec4(1.0f, 0.7f, 0.2f, 1.0f), "References:");
                ImGui::PopFont();// back to medium font again
                if(theBook.isLoaded) 
                {
                    float panel_width = ImGui::GetWindowWidth();
                    float image_width = panel_width * 0.2f; 
                    float image_height = image_width * ((float)theBook.height / (float)theBook.width); 
                    //ImGui::SetCursorPosX((panel_width - image_width) * 0.5f);
                    ImGui::Image((void*)(intptr_t)theBook.textureID, ImVec2(image_width, image_height));
                }
                else 
                {
                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Image could not be loaded!");// text in red color
                }
                ImGui::SameLine();
                ImGui::PushFont(largeFont);// set large font
                ImGui::TextWrapped("The underlying mathematical models and design equations utilized in this software are strictly based on the methodologies presented in \"Antenna Theory: Analysis and Design\" (4th Edition) by Constantine A. Balanis.");
                ImGui::PopFont();// back to medium font again
                ImGui::EndTabItem();
            }
            
            
            ImGui::EndTabBar();
        }
        ImGui::PopFont();// back to default font from medium font
        ImGui::End();
        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);        
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

