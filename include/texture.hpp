#pragma once

#include <string>
#include "image_interface.hpp"

enum Tex_Filtering {
    NEAREST = 0,
    LINEAR = 1,
    NEAREST_MIPMAP_NEAREST = 2,
    LINEAR_MIPMAP_NEAREST = 3,
    NEAREST_MIPMAP_LINEAR = 4,
    LINEAR_MIPMAP_LINEAR = 5
};

enum Tex_Filter_Level {
    TEXTURE_MIN_FILTER = 0,
    TEXTURE_MAG_FILTER = 1
};

enum Tex_Wrapping {
    REPEAT = 0,
    MIRRORED_REPEAT = 1,
    CLAMP_TO_EDGE = 2,
    CLAMP_TO_BORDER = 3
};

enum Tex_Wrap_Axis {
    TEXTURE_WRAP_S = 0,
    TEXTURE_WRAP_T = 1,
    TEXTURE_WRAP_R = 2
};

enum Tex_Target {
    TEXTURE_1D = 0,
    TEXTURE_2D = 1,
    TEXTURE_3D = 2,
    TEXTURE_RECTANGLE = 3,
    TEXTURE_BUFFER = 4,
    TEXTURE_CUBE_MAP = 5,
    TEXTURE_2D_MULTISAMPLE = 6
};

class Texture {
    protected:
        unsigned int mTextureID = 0;
        std::string mName = "";
        Tex_Wrapping mWrappingS = Tex_Wrapping::REPEAT;
        Tex_Wrapping mWrappingT = Tex_Wrapping::REPEAT;
        Tex_Wrapping mWrappingR = Tex_Wrapping::REPEAT;
        Tex_Filtering mFilteringMin = Tex_Filtering::NEAREST_MIPMAP_LINEAR;
        Tex_Filtering mFilteringMag = Tex_Filtering::LINEAR;
        Tex_Target mTarget = Tex_Target::TEXTURE_2D;

    public:
        // virtual Texture(std::string _name, std::string _filepath);
        // virtual ~Texture();
        virtual void Load(std::string _name, std::string _filepath) {}

        virtual int GetThumbWidth() { return 0; }
        virtual int GetThumbHeight() { return 0; }
        virtual float GetThumbAspectRatio() { return 0.0f; }

        void ReName(std::string _name) {
            this->mName = _name;
        }

        std::string GetName() {
            return this->mName;
        }

        const char* GetNameBuf() {
            return this->mName.c_str();
        }

        unsigned int GetID() {
            return this->mTextureID;
        }

        Tex_Target GetTarget() {
            return this->mTarget;
        }

        void SetFiltering(Tex_Filter_Level _level, Tex_Filtering _filter) {
            switch(_level) {
                case Tex_Filter_Level::TEXTURE_MIN_FILTER:
                    this->mFilteringMin = _filter;
                    break;
                case Tex_Filter_Level::TEXTURE_MAG_FILTER:
                    this->mFilteringMin = _filter;
                    break;
            }
        }

        void SetWrapping(Tex_Wrap_Axis _axis, Tex_Wrapping _wrap) {
            switch(_axis) {
                case Tex_Wrap_Axis::TEXTURE_WRAP_S:
                    this->mWrappingS = _wrap;
                    break;
                case Tex_Wrap_Axis::TEXTURE_WRAP_T:
                    this->mWrappingT = _wrap;
                    break;
                case Tex_Wrap_Axis::TEXTURE_WRAP_R:
                    this->mWrappingR = _wrap;
                    break;
            }
        }
        
};

enum TexDataType {
    INT = 0,
    FLOAT = 1
};

class Texture2D : public Texture {
    private:
        std::string mFilePath = "";
        int mHeight = 0;
        int mWidth = 0;
        int mChannels = 0;
        unsigned char* mData = NULL;
        float* mDataf = NULL;

    public:
        Texture2D(std::string _name, std::string _filepath, TexDataType _type) {
            this->mTarget = Tex_Target::TEXTURE_2D;
            Load(_name, _filepath, _type); 
        }

        ~Texture2D() {
            delete mData;
        }

        void Load(std::string _name, std::string _filepath, TexDataType _type) {
            mName = _name;
            glGenTextures(1, &mTextureID); 
            glBindTexture(GL_TEXTURE_2D, mTextureID);
            if (_type == TexDataType::INT) {
                mData = Image_Interface::LoadImage(_filepath.c_str(), &mWidth, &mHeight, &mChannels);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mData);
            } else {
                mDataf = Image_Interface::LoadImagef(_filepath.c_str(), &mWidth, &mHeight, &mChannels);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, mDataf);
            }
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        int GetThumbWidth() {
            return mWidth;
        }

        int GetThumbHeight() {
            return mHeight;
        }

        float GetThumbAspectRatio() {
            if (mHeight == 0)
                return 0.0f;
            return (float)mWidth / (float)mHeight;
        }
        
};

// GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
// GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
// GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
// GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Back
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Front

enum Cubemap_Face {
    POSITIVE_X = 0,
    NEGATIVE_X = 1,
    POSITIVE_Y = 2,
    NEGATIVE_Y = 3,
    POSITIVE_Z = 4,
    NEGATIVE_Z = 5
};

class Cubemap : public Texture {
    private:
        std::string mFilePaths[6] = {
            "", "", "", "", "", ""
        };
        int mHeight[6] = {
            0, 0, 0, 0, 0, 0
        };
        int mWidth[6] = {
            0, 0, 0, 0, 0, 0
        };
        int mChannels[6] = {
            0, 0, 0, 0, 0, 0
        };
        unsigned char* mData[6] = {
            NULL, NULL, NULL, NULL, NULL, NULL
        };

    public:
        Cubemap(std::string _name, std::string _filepath) {
            this->mTarget = Tex_Target::TEXTURE_CUBE_MAP;
            Load(_name, _filepath); 
        }

        ~Cubemap() {
            delete mData[0];
            delete mData[1];
            delete mData[2];
            delete mData[3];
            delete mData[4];
            delete mData[5];
            delete mData;
        }

        void Load(std::string _name, std::string _filepath) {
            mName = _name;
            for (int i = 0; i < 6; i++) {
                mData[i] = Image_Interface::LoadImage(_filepath.c_str(), &mWidth[i], &mHeight[i], &mChannels[i]);
            }
            
        }
        
};