/* ------------------------------------------------------------
name: "Distributed WFS"
Code generated with Faust 2.84.5 (https://faust.grame.fr)
Compilation options: -a ananas.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -dtl 1024 -es 1 -mcd 16 -mdd 1024 -mdy 33 -uim -single -ftz 0
------------------------------------------------------------ */

#ifndef  __mydsp_H__
#define  __mydsp_H__

#include "wfs.h"
#include <AnanasUtils.h>

#include <string.h> // for memset

// IMPORTANT: in order for MapUI to work, the teensy linker must be g++
/************************** BEGIN MapUI.h ******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***********************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <vector>
#include <map>
#include <string>
#include <stdio.h>

/************************** BEGIN UI.h *****************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***************************************************************************/

#ifndef __export__
#define __export__

// Version as a global string
#define FAUSTVERSION "2.84.5"

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 84
#define FAUSTPATCHVERSION 5

// Use FAUST_API for code that is part of the external API but is also compiled in faust and libfaust
// Use LIBFAUST_API for code that is compiled in faust and libfaust

#ifdef _WIN32
    #pragma warning (disable: 4251)
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #elif FAUST_LIB
        #define FAUST_API __declspec(dllexport)
        #define LIBFAUST_API __declspec(dllexport)
    #else
        #define FAUST_API
        #define LIBFAUST_API 
    #endif
#else
    #ifdef FAUST_EXE
        #define FAUST_API
        #define LIBFAUST_API
    #else
        #define FAUST_API __attribute__((visibility("default")))
        #define LIBFAUST_API __attribute__((visibility("default")))
    #endif
#endif

#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL>
struct FAUST_API UIReal {
    
    UIReal() {}
    virtual ~UIReal() {}
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // -- active widgets
    
    virtual void addButton(const char* label, REAL* zone) = 0;
    virtual void addCheckButton(const char* label, REAL* zone) = 0;
    virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;
    
    // -- metadata declarations
    
    virtual void declare(REAL* /*zone*/, const char* /*key*/, const char* /*val*/) {}

    // To be used by LLVM client
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct FAUST_API UI : public UIReal<FAUSTFLOAT> {
    UI() {}
    virtual ~UI() {}
#ifdef DAISY_NO_RTTI
    virtual bool isSoundUI() const { return false; }
    virtual bool isMidiInterface() const { return false; }
#endif
};

#endif
/**************************  END  UI.h **************************/
/************************** BEGIN PathBuilder.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __PathBuilder__
#define __PathBuilder__

#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>


/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class FAUST_API PathBuilder {

    protected:
    
        std::vector<std::string> fControlsLevel;
        std::vector<std::string> fFullPaths;
        std::map<std::string, std::string> fFull2Short;  // filled by computeShortNames()
    
        /**
         * @brief check if a character is acceptable for an ID
         *
         * @param c
         * @return true is the character is acceptable for an ID
         */
        bool isIDChar(char c) const
        {
            return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9'));
        }
    
        /**
         * @brief remove all "/0x00" parts
         *
         * @param src
         * @return modified string
         */
        std::string remove0x00(const std::string& src_aux) const
        {
            std::string src = src_aux;
            std::string from = "/0x00";
            std::string to = "";
            size_t pos = std::string::npos;
            while ((pos = src.find(from)) && (pos != std::string::npos)) {
                src = src.replace(pos, from.length(), to);
            }
            return src;
        }
    
        /**
         * @brief replace all non ID char with '_' (one '_' may replace several non ID char)
         *
         * @param src
         * @return modified string
         */
        std::string str2ID(const std::string& src) const
        {
            std::string dst;
            bool need_underscore = false;
            for (char c : src) {
                if (isIDChar(c) || (c == '/')) {
                    if (need_underscore) {
                        dst.push_back('_');
                        need_underscore = false;
                    }
                    dst.push_back(c);
                } else {
                    need_underscore = true;
                }
            }
            return dst;
        }
    
        /**
         * @brief Keep only the last n slash-parts
         *
         * @param src
         * @param n : 1 indicates the last slash-part
         * @return modified string
         */
        std::string cut(const std::string& src, int n) const
        {
            std::string rdst;
            for (int i = int(src.length())-1; i >= 0; i--) {
                char c = src[i];
                if (c != '/') {
                    rdst.push_back(c);
                } else if (n == 1) {
                    std::string dst;
                    for (int j = int(rdst.length())-1; j >= 0; j--) {
                        dst.push_back(rdst[j]);
                    }
                    return dst;
                } else {
                    n--;
                    rdst.push_back(c);
                }
            }
            return src;
        }
    
        void addFullPath(const std::string& label) { fFullPaths.push_back(buildPath(label)); }
    
        /**
         * @brief Compute the mapping between full path and short names
         */
        void computeShortNames()
        {
            std::vector<std::string>           uniquePaths;  // all full paths transformed but made unique with a prefix
            std::map<std::string, std::string> unique2full;  // all full paths transformed but made unique with a prefix
            char num_buffer[16];
            int pnum = 0;
            
            for (const auto& s : fFullPaths) {
                // Using snprintf since Teensy does not have the std::to_string function
                snprintf(num_buffer, 16, "%d", pnum++);
                std::string u = "/P" + std::string(num_buffer) + str2ID(remove0x00(s));
                uniquePaths.push_back(u);
                unique2full[u] = s;  // remember the full path associated to a unique path
            }
        
            std::map<std::string, int> uniquePath2level;                // map path to level
            for (const auto& s : uniquePaths) uniquePath2level[s] = 1;   // we init all levels to 1
            bool have_collisions = true;
        
            while (have_collisions) {
                // compute collision list
                std::set<std::string>              collisionSet;
                std::map<std::string, std::string> short2full;
                have_collisions = false;
                for (const auto& it : uniquePath2level) {
                    std::string u = it.first;
                    int n = it.second;
                    std::string shortName = cut(u, n);
                    auto p = short2full.find(shortName);
                    if (p == short2full.end()) {
                        // no collision
                        short2full[shortName] = u;
                    } else {
                        // we have a collision, add the two paths to the collision set
                        have_collisions = true;
                        collisionSet.insert(u);
                        collisionSet.insert(p->second);
                    }
                }
                for (const auto& s : collisionSet) uniquePath2level[s]++;  // increase level of colliding path
            }
        
            for (const auto& it : uniquePath2level) {
                std::string u = it.first;
                int n = it.second;
                std::string shortName = replaceCharList(cut(u, n), {'/'}, '_');
                fFull2Short[unique2full[u]] = shortName;
            }
        }
    
        std::string replaceCharList(const std::string& str, const std::vector<char>& ch1, char ch2)
        {
            auto beg = ch1.begin();
            auto end = ch1.end();
            std::string res = str;
            for (size_t i = 0; i < str.length(); ++i) {
                if (std::find(beg, end, str[i]) != end) res[i] = ch2;
            }
            return res;
        }
     
    public:
    
        PathBuilder() {}
        virtual ~PathBuilder() {}
    
        // Return true for the first level of groups
        bool pushLabel(const std::string& label_aux)
        {
            std::string label = replaceCharList(label_aux, {'/'}, '_');
            fControlsLevel.push_back(label); return fControlsLevel.size() == 1;
        }
    
        // Return true for the last level of groups
        bool popLabel() { fControlsLevel.pop_back(); return fControlsLevel.size() == 0; }
    
        // Return a complete path built from a label
        std::string buildPath(const std::string& label_aux)
        {
            std::string label = replaceCharList(label_aux, {'/'}, '_');
            std::string res = "/";
            for (size_t i = 0; i < fControlsLevel.size(); i++) {
                res = res + fControlsLevel[i] + "/";
            }
            res += label;
            return replaceCharList(res, {' ', '#', '*', ',', '?', '[', ']', '{', '}', '(', ')'}, '_');
        }
    
        // Assuming shortnames have been built, return the shortname from a label
        std::string buildShortname(const std::string& label)
        {
            return (hasShortname()) ? fFull2Short[buildPath(label)] : "";
        }
    
        bool hasShortname() { return fFull2Short.size() > 0; }
    
};

#endif  // __PathBuilder__
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of unique 'shortname' (built so that they never collide) and zones for each UI item
 * - a map of complete hierarchical 'paths' and zones for each UI item
 *
 * Simple 'labels', 'shortname' and complete 'paths' (to fully discriminate between possible same
 * 'labels' at different location in the UI hierachy) can be used to access a given parameter.
 ******************************************************************************/

class FAUST_API MapUI : public UI, public PathBuilder
{
    
    protected:
    
        // Label zone map
        std::map<std::string, FAUSTFLOAT*> fLabelZoneMap;
    
        // Shortname zone map
        std::map<std::string, FAUSTFLOAT*> fShortnameZoneMap;
    
        // Full path map
        std::map<std::string, FAUSTFLOAT*> fPathZoneMap;
    
        void addZoneLabel(const std::string& label, FAUSTFLOAT* zone)
        {
            std::string path = buildPath(label);
            fFullPaths.push_back(path);
            fPathZoneMap[path] = zone;
            fLabelZoneMap[label] = zone;
        }
    
    public:
        
        MapUI() {}
        virtual ~MapUI() {}
        
        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            if (popLabel()) {
                // Shortnames can be computed when all fullnames are known
                computeShortNames();
                // Fill 'shortname' map
                for (const auto& it : fFullPaths) {
                    fShortnameZoneMap[fFull2Short[it]] = fPathZoneMap[it];
                }
            }
        }
        
        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addZoneLabel(label, zone);
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addZoneLabel(label, zone);
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            addZoneLabel(label, zone);
        }
        
        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            addZoneLabel(label, zone);
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            addZoneLabel(label, zone);
        }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
        
        // -- metadata declarations
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {}
    
        //-------------------------------------------------------------------------------
        // Public API
        //-------------------------------------------------------------------------------
    
        /**
         * Set the param value.
         *
         * @param str - the UI parameter label/shortname/path
         * @param value - the UI parameter value
         *
         */
        void setParamValue(const std::string& str, FAUSTFLOAT value)
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                *fPathZoneMapIter->second = value;
                return;
            }
            
            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                *fShortnameZoneMapIter->second = value;
                return;
            }
            
            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                *fLabelZoneMapIter->second = value;
                return;
            }
            
            fprintf(stderr, "ERROR : setParamValue '%s' not found\n", str.c_str());
        }
        
        /**
         * Return the param value.
         *
         * @param str - the UI parameter label/shortname/path
         *
         * @return the param value.
         */
        FAUSTFLOAT getParamValue(const std::string& str) const
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                return *fPathZoneMapIter->second;
            }
            
            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                return *fShortnameZoneMapIter->second;
            }
            
            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                return *fLabelZoneMapIter->second;
            }
            
            fprintf(stderr, "ERROR : getParamValue '%s' not found\n", str.c_str());
            return 0;
        }
    
        // map access 
        std::map<std::string, FAUSTFLOAT*>& getFullpathMap() { return fPathZoneMap; }
        std::map<std::string, FAUSTFLOAT*>& getShortnameMap() { return fShortnameZoneMap; }
        std::map<std::string, FAUSTFLOAT*>& getLabelMap() { return fLabelZoneMap; }
            
        /**
         * Return the number of parameters in the UI.
         *
         * @return the number of parameters
         */
        int getParamsCount() const { return int(fPathZoneMap.size()); }
        
        /**
         * Return the param path.
         *
         * @param index - the UI parameter index
         *
         * @return the param path
         */
        std::string getParamAddress(int index) const
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return "";
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamAddress1(int index) const
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        /**
         * Return the param shortname.
         *
         * @param index - the UI parameter index
         *
         * @return the param shortname
         */
        std::string getParamShortname(int index) const
        {
            if (index < 0 || index > int(fShortnameZoneMap.size())) {
                return "";
            } else {
                auto it = fShortnameZoneMap.begin();
                while (index-- > 0 && it++ != fShortnameZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamShortname1(int index) const
        {
            if (index < 0 || index > int(fShortnameZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fShortnameZoneMap.begin();
                while (index-- > 0 && it++ != fShortnameZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        /**
         * Return the param label.
         *
         * @param index - the UI parameter index
         *
         * @return the param label
         */
        std::string getParamLabel(int index) const
        {
            if (index < 0 || index > int(fLabelZoneMap.size())) {
                return "";
            } else {
                auto it = fLabelZoneMap.begin();
                while (index-- > 0 && it++ != fLabelZoneMap.end()) {}
                return it->first;
            }
        }
        
        const char* getParamLabel1(int index) const
        {
            if (index < 0 || index > int(fLabelZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fLabelZoneMap.begin();
                while (index-- > 0 && it++ != fLabelZoneMap.end()) {}
                return it->first.c_str();
            }
        }
    
        /**
         * Return the param path.
         *
         * @param zone - the UI parameter memory zone
         *
         * @return the param path
         */
        std::string getParamAddress(FAUSTFLOAT* zone) const
        {
            for (const auto& it : fPathZoneMap) {
                if (it.second == zone) return it.first;
            }
            return "";
        }
    
        /**
         * Return the param memory zone.
         *
         * @param zone - the UI parameter label/shortname/path
         *
         * @return the param path
         */
        FAUSTFLOAT* getParamZone(const std::string& str) const
        {
            const auto fPathZoneMapIter = fPathZoneMap.find(str);
            if (fPathZoneMapIter != fPathZoneMap.end()) {
                return fPathZoneMapIter->second;
            }
            
            const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
            if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
                return fShortnameZoneMapIter->second;
            }
            
            const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
            if (fLabelZoneMapIter != fLabelZoneMap.end()) {
                return fLabelZoneMapIter->second;
            }

            return nullptr;
        }
    
        /**
         * Return the param memory zone.
         *
         * @param zone - the UI parameter index
         *
         * @return the param path
         */
        FAUSTFLOAT* getParamZone(int index) const 
        {
            if (index < 0 || index > int(fPathZoneMap.size())) {
                return nullptr;
            } else {
                auto it = fPathZoneMap.begin();
                while (index-- > 0 && it++ != fPathZoneMap.end()) {}
                return it->second;
            }
        }
    
        static bool endsWith(const std::string& str, const std::string& end)
        {
            size_t l1 = str.length();
            size_t l2 = end.length();
            return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
        }
    
};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN meta.h *******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__


/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to retrieve (key, value) metadata.
 */
struct FAUST_API Meta {
    virtual ~Meta() {}
    virtual void declare(const char* key, const char* value) = 0;
};

#endif
/**************************  END  meta.h **************************/
/************************** BEGIN dsp.h ********************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp__
#define __dsp__

#include <string>
#include <vector>
#include <cstdint>


#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct FAUST_API UI;
struct FAUST_API Meta;

/**
 * DSP memory manager.
 */

struct FAUST_API dsp_memory_manager {
    
    enum MemType { kInt32, kInt32_ptr, kFloat, kFloat_ptr, kDouble, kDouble_ptr, kQuad, kQuad_ptr, kFixedPoint, kFixedPoint_ptr, kObj, kObj_ptr, kSound, kSound_ptr };

    virtual ~dsp_memory_manager() = default;
    
    /**
     * Inform the Memory Manager with the number of expected memory zones.
     * @param count - the number of expected memory zones
     */
    virtual void begin(size_t count) {}
    
    /**
     * Give the Memory Manager information on a given memory zone.
     * @param name - the memory zone name
     * @param type - the memory zone type (in MemType)
     * @param size - the size in unit of the memory type of the memory zone
     * @param size_bytes - the size in bytes of the memory zone
     * @param reads - the number of Read access to the zone used to compute one frame
     * @param writes - the number of Write access to the zone used to compute one frame
     */
    virtual void info(const char* name, MemType type, size_t size, size_t size_bytes, size_t reads, size_t writes) {}
  
    /**
     * Inform the Memory Manager that all memory zones have been described,
     * to possibly start a 'compute the best allocation strategy' step.
     */
    virtual void end() {}
    
    /**
     * Allocate a memory zone.
     * @param size - the memory zone size in bytes
     */
    virtual void* allocate(size_t size) = 0;
    
    /**
     * Destroy a memory zone.
     * @param ptr - the memory zone pointer to be deallocated
     */
    virtual void destroy(void* ptr) = 0;
    
};

/**
* Signal processor definition.
*/

class FAUST_API dsp {

    public:

        dsp() = default;
        virtual ~dsp() = default;

        /* Return instance number of audio inputs */
        virtual int getNumInputs() = 0;
    
        /* Return instance number of audio outputs */
        virtual int getNumOutputs() = 0;
    
        /**
         * Trigger the ui_interface parameter with instance specific calls
         * to 'openTabBox', 'addButton', 'addVerticalSlider'... in order to build the UI.
         *
         * @param ui_interface - the user interface builder
         */
        virtual void buildUserInterface(UI* ui_interface) = 0;
    
        /* Return the sample rate currently used by the instance */
        virtual int getSampleRate() = 0;
    
        /**
         * Global init, calls the following methods:
         * - static class 'classInit': static tables initialization
         * - 'instanceInit': constants and instance state initialization
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void init(int sample_rate) = 0;

        /**
         * Init instance state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceInit(int sample_rate) = 0;
    
        /**
         * Init instance constant state.
         *
         * @param sample_rate - the sampling rate in Hz
         */
        virtual void instanceConstants(int sample_rate) = 0;
    
        /* Init default control parameters values */
        virtual void instanceResetUserInterface() = 0;
    
        /* Init instance state (like delay lines...) but keep the control parameter values */
        virtual void instanceClear() = 0;
 
        /**
         * Return a clone of the instance.
         *
         * @return a copy of the instance on success, otherwise a null pointer.
         */
        virtual ::dsp* clone() = 0;
    
        /**
         * Trigger the Meta* m parameter with instance specific calls to 'declare' (key, value) metadata.
         *
         * @param m - the Meta* meta user
         */
        virtual void metadata(Meta* m) = 0;

    
        /**
         * Read all controllers (buttons, sliders, etc.), and update the DSP state to be used by 'frame' or 'compute'.
         * This method will be filled with the -ec (--external-control) option.
         */
        virtual void control() {}
    
        /**
         * DSP instance computation to process one single frame.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write frame(inputs, inputs).
         * The -inpl option can be used for that, but only in scalar mode for now.
         * This method will be filled with the -os (--one-sample) option.
         *
         * @param inputs - the input audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         * @param outputs - the output audio buffers as an array of FAUSTFLOAT samples (eiher float, double or quad)
         */
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) {}
        
        /**
         * DSP instance computation to be called with successive in/out audio buffers.
         *
         * Note that by default inputs and outputs buffers are supposed to be distinct memory zones,
         * so one cannot safely write compute(count, inputs, inputs).
         * The -inpl compilation option can be used for that, but only in scalar mode for now.
         *
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT buffers
         * (containing either float, double or quad samples)
         */
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
    
        /**
         * Alternative DSP instance computation method for use by subclasses, incorporating an additional `date_usec` parameter,
         * which specifies the timestamp of the first sample in the audio buffers.
         *
         * @param date_usec - the timestamp in microsec given by audio driver. By convention timestamp of -1 means 'no timestamp conversion',
         * events already have a timestamp expressed in frames.
         * @param count - the number of frames to compute
         * @param inputs - the input audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         * @param outputs - the output audio buffers as an array of non-interleaved FAUSTFLOAT samples (either float, double or quad)
         */
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
       
};

/**
 * Generic DSP decorator.
 */

class FAUST_API decorator_dsp : public ::dsp {

    protected:

        ::dsp* fDSP;

    public:

        decorator_dsp(::dsp* dsp = nullptr):fDSP(dsp) {}
        virtual ~decorator_dsp() { delete fDSP; }

        virtual int getNumInputs() override { return fDSP->getNumInputs(); }
        virtual int getNumOutputs() override { return fDSP->getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) override { fDSP->buildUserInterface(ui_interface); }
        virtual int getSampleRate() override { return fDSP->getSampleRate(); }
        virtual void init(int sample_rate) override { fDSP->init(sample_rate); }
        virtual void instanceInit(int sample_rate) override { fDSP->instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) override { fDSP->instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() override { fDSP->instanceResetUserInterface(); }
        virtual void instanceClear() override { fDSP->instanceClear(); }
        virtual decorator_dsp* clone() override { return new decorator_dsp(fDSP->clone()); }
        virtual void metadata(Meta* m) override { fDSP->metadata(m); }
        // Beware: subclasses usually have to overload the two 'compute' methods
        virtual void control() override { fDSP->control(); }
        virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) override { fDSP->frame(inputs, outputs); }
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) override { fDSP->compute(count, inputs, outputs); }
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) override { fDSP->compute(date_usec, count, inputs, outputs); }
    
};

/**
 * DSP factory class, used with LLVM and Interpreter backends
 * to create DSP instances from a compiled DSP program.
 */

class FAUST_API dsp_factory {
    
    protected:
    
        // So that to force sub-classes to use deleteDSPFactory(dsp_factory* factory);
        virtual ~dsp_factory() = default;
    
    public:
    
        /* Return factory name */
        virtual std::string getName() = 0;
    
        /* Return factory SHA key */
        virtual std::string getSHAKey() = 0;
    
        /* Return factory expanded DSP code */
        virtual std::string getDSPCode() = 0;
    
        /* Return factory compile options */
        virtual std::string getCompileOptions() = 0;
    
        /* Get the Faust DSP factory list of library dependancies */
        virtual std::vector<std::string> getLibraryList() = 0;
    
        /* Get the list of all used includes */
        virtual std::vector<std::string> getIncludePathnames() = 0;
    
        /* Get warning messages list for a given compilation */
        virtual std::vector<std::string> getWarningMessages() = 0;

        /* Return JSON description of the DSP (UI + metadata) */
        virtual std::string getJSON() = 0;
    
        /* Create a new DSP instance, to be deleted with C++ 'delete' */
        virtual ::dsp* createDSPInstance() = 0;
    
        /* Static tables initialization, possibly implemened in sub-classes*/
        virtual void classInit(int sample_rate) {};
    
        /* Set a custom memory manager to be used when creating instances */
        virtual void setMemoryManager(dsp_memory_manager* manager) = 0;
    
        /* Return the currently set custom memory manager */
        virtual dsp_memory_manager* getMemoryManager() = 0;
    
};

// Denormal handling

#if defined (__SSE__)
#include <xmmintrin.h>
#endif

class FAUST_API ScopedNoDenormals {
    
    private:
    
        intptr_t fpsr = 0;
        
        void setFpStatusRegister(intptr_t fpsr_aux) noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("msr fpcr, %0" : : "ri" (fpsr_aux));
        #elif defined (__SSE__)
            // The volatile keyword here is needed to workaround a bug in AppleClang 13.0
            // which aggressively optimises away the variable otherwise
            volatile uint32_t fpsr_w = static_cast<uint32_t>(fpsr_aux);
            _mm_setcsr(fpsr_w);
        #endif
        }
        
        void getFpStatusRegister() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            asm volatile("mrs %0, fpcr" : "=r" (fpsr));
        #elif defined (__SSE__)
            fpsr = static_cast<intptr_t>(_mm_getcsr());
        #endif
        }
    
    public:
    
        ScopedNoDenormals() noexcept
        {
        #if defined (__arm64__) || defined (__aarch64__)
            intptr_t mask = (1 << 24 /* FZ */);
        #elif defined (__SSE__)
        #if defined (__SSE2__)
            intptr_t mask = 0x8040;
        #else
            intptr_t mask = 0x8000;
        #endif
        #else
            intptr_t mask = 0x0000;
        #endif
            getFpStatusRegister();
            setFpStatusRegister(fpsr | mask);
        }
        
        ~ScopedNoDenormals() noexcept
        {
            setFpStatusRegister(fpsr);
        }

};

#define AVOIDDENORMALS ScopedNoDenormals ftz_scope;

#endif

/************************** END dsp.h **************************/

// MIDI support
#if MIDICTRL
/************************** BEGIN MidiUI.h ****************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
************************************************************************/

#ifndef FAUST_MIDIUI_H
#define FAUST_MIDIUI_H

#include <vector>
#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>

/************************** BEGIN GUI.h **********************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 *************************************************************************/

#ifndef __GUI_H__
#define __GUI_H__

#include <list>
#include <map>
#include <vector>
#include <assert.h>

#ifdef _WIN32
# pragma warning (disable: 4100)
#else
# pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

/************************** BEGIN ValueConverter.h ********************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __ValueConverter__
#define __ValueConverter__

/***************************************************************************************
 ValueConverter.h
 (GRAME, Copyright 2015-2019)
 
 Set of conversion objects used to map user interface values (for example a gui slider
 delivering values between 0 and 1) to Faust values (for example a vslider between
 20 and 20000) using a log scale.
 
 -- Utilities
 
 Range(lo,hi) : clip a value x between lo and hi
 Interpolator(lo,hi,v1,v2) : Maps a value x between lo and hi to a value y between v1 and v2
 Interpolator3pt(lo,mi,hi,v1,vm,v2) : Map values between lo mid hi to values between v1 vm v2
 
 -- Value Converters
 
 ValueConverter::ui2faust(x)
 ValueConverter::faust2ui(x)
 
 -- ValueConverters used for sliders depending of the scale
 
 LinearValueConverter(umin, umax, fmin, fmax)
 LinearValueConverter2(lo, mi, hi, v1, vm, v2) using 2 segments
 LogValueConverter(umin, umax, fmin, fmax)
 ExpValueConverter(umin, umax, fmin, fmax)
 
 -- ValueConverters used for accelerometers based on 3 points
 
 UpConverter(amin, amid, amax, fmin, fmid, fmax)        -- curve 0
 DownConverter(amin, amid, amax, fmin, fmid, fmax)      -- curve 1
 UpDownConverter(amin, amid, amax, fmin, fmid, fmax)    -- curve 2
 DownUpConverter(amin, amid, amax, fmin, fmid, fmax)    -- curve 3
 
 -- lists of ZoneControl are used to implement accelerometers metadata for each axes
 
 ZoneControl(zone, valueConverter) : a zone with an accelerometer data converter
 
 -- ZoneReader are used to implement screencolor metadata
 
 ZoneReader(zone, valueConverter) : a zone with a data converter

****************************************************************************************/

#include <float.h>
#include <algorithm>    // std::max
#include <cmath>
#include <vector>
#include <assert.h>


//--------------------------------------------------------------------------------------
// Interpolator(lo,hi,v1,v2)
// Maps a value x between lo and hi to a value y between v1 and v2
// y = v1 + (x-lo)/(hi-lo)*(v2-v1)
// y = v1 + (x-lo) * coef           with coef = (v2-v1)/(hi-lo)
// y = v1 + x*coef - lo*coef
// y = v1 - lo*coef + x*coef
// y = offset + x*coef              with offset = v1 - lo*coef
//--------------------------------------------------------------------------------------
class FAUST_API Interpolator {
    
    private:

        //--------------------------------------------------------------------------------------
        // Range(lo,hi) clip a value between lo and hi
        //--------------------------------------------------------------------------------------
        struct Range
        {
            double fLo;
            double fHi;

            Range(double x, double y) : fLo(std::min<double>(x,y)), fHi(std::max<double>(x,y)) {}
            double operator()(double x) const noexcept { return (x<fLo) ? fLo : (x>fHi) ? fHi : x; }
        };

        Range fRange;
        double fCoef;
        double fOffset;

    public:

        Interpolator(double lo, double hi, double v1, double v2) : fRange(lo,hi)
        {
            if (hi != lo) {
                // regular case
                fCoef = (v2-v1)/(hi-lo);
                fOffset = v1 - lo*fCoef;
            } else {
                // degenerate case, avoids division by zero
                fCoef = 0;
                fOffset = (v1+v2)/2;
            }
        }
        double operator()(double v) const noexcept
        {
            double x = fRange(v);
            return  fOffset + x*fCoef;
        }

        double coef() const noexcept { return fCoef; }

        void getLowHigh(double& amin, double& amax) const noexcept
        {
            amin = fRange.fLo;
            amax = fRange.fHi;
        }
};

//--------------------------------------------------------------------------------------
// Interpolator3pt(lo,mi,hi,v1,vm,v2)
// Map values between lo mid hi to values between v1 vm v2
//--------------------------------------------------------------------------------------
class FAUST_API Interpolator3pt {

    private:

        Interpolator fSegment1;
        Interpolator fSegment2;
        double fMid;

    public:

        Interpolator3pt(double lo, double mi, double hi, double v1, double vm, double v2) :
            fSegment1(lo, mi, v1, vm),
            fSegment2(mi, hi, vm, v2),
            fMid(mi) {}
        double operator()(double x) const noexcept { return  (x < fMid) ? fSegment1(x) : fSegment2(x); }

        void getMappingValues(double& amin, double& amid, double& amax) const noexcept
        {
            fSegment1.getLowHigh(amin, amid);
            fSegment2.getLowHigh(amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Abstract ValueConverter class. Converts values between UI and Faust representations
//--------------------------------------------------------------------------------------
class FAUST_API ValueConverter {

    public:

        virtual ~ValueConverter() {}
        virtual double ui2faust(double x) { return x; };
        virtual double faust2ui(double x) { return x; };
};

//--------------------------------------------------------------------------------------
// A converter than can be updated
//--------------------------------------------------------------------------------------

class FAUST_API UpdatableValueConverter : public ValueConverter {
    
    protected:
        
        bool fActive;
        
    public:
        
        UpdatableValueConverter():fActive(true)
        {}
        virtual ~UpdatableValueConverter()
        {}
        
        virtual void setMappingValues(double amin, double amid, double amax, double min, double init, double max) = 0;
        virtual void getMappingValues(double& amin, double& amid, double& amax) = 0;
        
        void setActive(bool on_off) { fActive = on_off; }
        bool getActive() { return fActive; }
    
};

//--------------------------------------------------------------------------------------
// Linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API LinearValueConverter : public ValueConverter {
    
    private:
        
        Interpolator fUI2F;
        Interpolator fF2UI;
        
    public:
        
        LinearValueConverter(double umin, double umax, double fmin, double fmax) :
            fUI2F(umin,umax,fmin,fmax), fF2UI(fmin,fmax,umin,umax)
        {}
        
        LinearValueConverter() : fUI2F(0.,0.,0.,0.), fF2UI(0.,0.,0.,0.)
        {}
        virtual double ui2faust(double x) { return fUI2F(x); }
        virtual double faust2ui(double x)
        {
            // Avoid division by zero; if coef is zero, fall back to midpoint in the clamped range.
            if (fF2UI.coef() != 0.0) {
                return fF2UI(x);
            } else {
                double lo, hi;
                fF2UI.getLowHigh(lo, hi);
                double mid = (lo + hi) / 2.0;
                return fF2UI(mid);
            }
        }
    
};

//--------------------------------------------------------------------------------------
// Two segments linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API LinearValueConverter2 : public UpdatableValueConverter {
    
    private:
    
        Interpolator3pt fUI2F;
        Interpolator3pt fF2UI;
        
    public:
    
        LinearValueConverter2(double amin, double amid, double amax, double min, double init, double max) :
            fUI2F(amin, amid, amax, min, init, max), fF2UI(min, init, max, amin, amid, amax)
        {}
        
        LinearValueConverter2() : fUI2F(0.,0.,0.,0.,0.,0.), fF2UI(0.,0.,0.,0.,0.,0.)
        {}
    
        virtual double ui2faust(double x) { return fUI2F(x); }
        virtual double faust2ui(double x) { return fF2UI(x); }
    
        virtual void setMappingValues(double amin, double amid, double amax, double min, double init, double max)
        {
            fUI2F = Interpolator3pt(amin, amid, amax, min, init, max);
            fF2UI = Interpolator3pt(min, init, max, amin, amid, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fUI2F.getMappingValues(amin, amid, amax);
        }
    
};

//--------------------------------------------------------------------------------------
// Logarithmic conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API LogValueConverter : public LinearValueConverter {

    public:
    
        // We use DBL_EPSILON which is bigger than DBL_MIN (safer)
        LogValueConverter(double umin, double umax, double fmin, double fmax) :
            LinearValueConverter(umin, umax, std::log(std::max<double>(DBL_EPSILON, fmin)), std::log(std::max<double>(DBL_EPSILON, fmax)))
        {}

        virtual double ui2faust(double x) { return std::exp(LinearValueConverter::ui2faust(x)); }
        virtual double faust2ui(double x) { return LinearValueConverter::faust2ui(std::log(std::max<double>(DBL_EPSILON, x))); }

};

//--------------------------------------------------------------------------------------
// Exponential conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API ExpValueConverter : public LinearValueConverter {

    public:

        ExpValueConverter(double umin, double umax, double fmin, double fmax) :
            LinearValueConverter(umin, umax, std::min<double>(DBL_MAX, std::exp(fmin)), std::min<double>(DBL_MAX, std::exp(fmax)))
        {}

        virtual double ui2faust(double x) { return std::log(LinearValueConverter::ui2faust(x)); }
        virtual double faust2ui(double x) { return LinearValueConverter::faust2ui(std::min<double>(DBL_MAX, std::exp(x))); }

};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up curve (curve 0)
//--------------------------------------------------------------------------------------
class FAUST_API UpConverter : public UpdatableValueConverter {

    private:

        Interpolator3pt fA2F;
        Interpolator3pt fF2A;

    public:

        UpConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmin,fmid,fmax),
            fF2A(fmin,fmid,fmax,amin,amid,amax)
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "UpConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmin, fmid, fmax);
            fF2A = Interpolator3pt(fmin, fmid, fmax, amin, amid, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }

};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down curve (curve 1)
//--------------------------------------------------------------------------------------
class FAUST_API DownConverter : public UpdatableValueConverter {

    private:

        Interpolator3pt	fA2F;
        Interpolator3pt	fF2A;

    public:

        DownConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmax,fmid,fmin),
            fF2A(fmin,fmid,fmax,amax,amid,amin)
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
             //__android_log_print(ANDROID_LOG_ERROR, "Faust", "DownConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmax, fmid, fmin);
            fF2A = Interpolator3pt(fmin, fmid, fmax, amax, amid, amin);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up-Down curve (curve 2)
//--------------------------------------------------------------------------------------
class FAUST_API UpDownConverter : public UpdatableValueConverter {

    private:

        Interpolator3pt	fA2F;
        Interpolator fF2A;

    public:

        UpDownConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmin,fmax,fmin),
            fF2A(fmin,fmax,amin,amax)				// Special, pseudo inverse of a non monotonic function
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "UpDownConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmin, fmax, fmin);
            fF2A = Interpolator(fmin, fmax, amin, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down-Up curve (curve 3)
//--------------------------------------------------------------------------------------
class FAUST_API DownUpConverter : public UpdatableValueConverter {

    private:

        Interpolator3pt	fA2F;
        Interpolator fF2A;

    public:

        DownUpConverter(double amin, double amid, double amax, double fmin, double fmid, double fmax) :
            fA2F(amin,amid,amax,fmax,fmin,fmax),
            fF2A(fmin,fmax,amin,amax)				// Special, pseudo inverse of a non monotonic function
        {}

        virtual double ui2faust(double x) { return fA2F(x); }
        virtual double faust2ui(double x) { return fF2A(x); }

        virtual void setMappingValues(double amin, double amid, double amax, double fmin, double fmid, double fmax)
        {
            //__android_log_print(ANDROID_LOG_ERROR, "Faust", "DownUpConverter update %f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
            fA2F = Interpolator3pt(amin, amid, amax, fmax, fmin, fmax);
            fF2A = Interpolator(fmin, fmax, amin, amax);
        }

        virtual void getMappingValues(double& amin, double& amid, double& amax)
        {
            fA2F.getMappingValues(amin, amid, amax);
        }
};

//--------------------------------------------------------------------------------------
// Base class for ZoneControl
//--------------------------------------------------------------------------------------
class FAUST_API ZoneControl {

    protected:

        FAUSTFLOAT*	fZone;

    public:

        ZoneControl(FAUSTFLOAT* zone) : fZone(zone) {}
        virtual ~ZoneControl() {}

        virtual void update(double v) const {}

        virtual void setMappingValues(int curve, double amin, double amid, double amax, double min, double init, double max) {}
        virtual void getMappingValues(double& amin, double& amid, double& amax) {}

        FAUSTFLOAT* getZone() { return fZone; }

        virtual void setActive(bool on_off) {}
        virtual bool getActive() { return false; }

        virtual int getCurve() { return -1; }

};

//--------------------------------------------------------------------------------------
//  Useful to implement accelerometers metadata as a list of ZoneControl for each axes
//--------------------------------------------------------------------------------------
class FAUST_API ConverterZoneControl : public ZoneControl {

    protected:

        ValueConverter* fValueConverter;

    public:

        ConverterZoneControl(FAUSTFLOAT* zone, ValueConverter* converter) : ZoneControl(zone), fValueConverter(converter) {}
        virtual ~ConverterZoneControl() { delete fValueConverter; } // Assuming fValueConverter is not kept elsewhere...

        virtual void update(double v) const { *fZone = FAUSTFLOAT(fValueConverter->ui2faust(v)); }

        ValueConverter* getConverter() { return fValueConverter; }

};

//--------------------------------------------------------------------------------------
// Association of a zone and a four value converter, each one for each possible curve.
// Useful to implement accelerometers metadata as a list of ZoneControl for each axes
//--------------------------------------------------------------------------------------
class FAUST_API CurveZoneControl : public ZoneControl {

    private:

        std::vector<UpdatableValueConverter*> fValueConverters;
        int fCurve;

    public:

        CurveZoneControl(FAUSTFLOAT* zone, int curve, double amin, double amid, double amax, double min, double init, double max) : ZoneControl(zone), fCurve(0)
        {
            assert(curve >= 0 && curve <= 3);
            fValueConverters.push_back(new UpConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new DownConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new UpDownConverter(amin, amid, amax, min, init, max));
            fValueConverters.push_back(new DownUpConverter(amin, amid, amax, min, init, max));
            fCurve = curve;
        }
    
        virtual ~CurveZoneControl()
        {
            for (const auto& it : fValueConverters) { delete it; }
        }
    
        void update(double v) const { if (fValueConverters[fCurve]->getActive()) *fZone = FAUSTFLOAT(fValueConverters[fCurve]->ui2faust(v)); }

        void setMappingValues(int curve, double amin, double amid, double amax, double min, double init, double max)
        {
            fValueConverters[curve]->setMappingValues(amin, amid, amax, min, init, max);
            fCurve = curve;
        }

        void getMappingValues(double& amin, double& amid, double& amax)
        {
            fValueConverters[fCurve]->getMappingValues(amin, amid, amax);
        }

        void setActive(bool on_off)
        {
            for (const auto& it : fValueConverters) { it->setActive(on_off); }
        }

        int getCurve() { return fCurve; }
};

class FAUST_API ZoneReader {

    private:

        FAUSTFLOAT* fZone;
        Interpolator fInterpolator;

    public:

        ZoneReader(FAUSTFLOAT* zone, double lo, double hi) : fZone(zone), fInterpolator(lo, hi, 0, 255) {}

        virtual ~ZoneReader() {}

        int getValue()
        {
            return (fZone != nullptr) ? int(fInterpolator(*fZone)) : 127;
        }

};

#endif
/**************************  END  ValueConverter.h **************************/
/************************** BEGIN MetaDataUI.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef MetaData_UI_H
#define MetaData_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <map>
#include <set>
#include <string>
#include <string.h>
#include <assert.h>
#include <stdio.h> // We use the lighter fprintf code

/************************** BEGIN SimpleParser.h *********************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

// ---------------------------------------------------------------------
//                          Simple Parser
// A parser returns true if it was able to parse what it is
// supposed to parse and advance the pointer. Otherwise it returns false
// and the pointer is not advanced so that another parser can be tried.
// ---------------------------------------------------------------------

#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdio.h> // We use the lighter fprintf code
#include <ctype.h>
#include <assert.h>

#ifndef _WIN32
# pragma GCC diagnostic ignored "-Wunused-function"
#endif

struct itemInfo {
    std::string type;
    std::string label;
    std::string varname;
    std::string shortname;
    std::string address;
    std::string url;
    int index;
    double init;
    double fmin;
    double fmax;
    double step;
    std::vector<std::pair<std::string, std::string> > meta;
    
    itemInfo():index(0), init(0.), fmin(0.), fmax(0.), step(0.)
    {}
};

// ---------------------------------------------------------------------
//                          Elementary parsers
// ---------------------------------------------------------------------

// Report a parsing error
static bool parseError(const char*& p, const char* errmsg)
{
    fprintf(stderr, "Parse error : %s here : %s\n", errmsg, p);
    return true;
}

/**
 * @brief skipBlank : advance pointer p to the first non blank character
 * @param p the string to parse, then the remaining string
 */
static void skipBlank(const char*& p)
{
    while (isspace(*p)) { p++; }
}

// Parse character x, but don't report error if fails
static bool tryChar(const char*& p, char x)
{
    skipBlank(p);
    if (x == *p) {
        p++;
        return true;
    } else {
        return false;
    }
}

/**
 * @brief parseChar : parse a specific character x
 * @param p the string to parse, then the remaining string
 * @param x the character to recognize
 * @return true if x was found at the begin of p
 */
static bool parseChar(const char*& p, char x)
{
    skipBlank(p);
    if (x == *p) {
        p++;
        return true;
    } else {
        return false;
    }
}

/**
 * @brief parseWord : parse a specific string w
 * @param p the string to parse, then the remaining string
 * @param w the string to recognize
 * @return true if string w was found at the begin of p
 */
static bool parseWord(const char*& p, const char* w)
{
    skipBlank(p);
    const char* saved = p;  // to restore position if we fail
    while ((*w == *p) && (*w)) {++w; ++p;}
    if (*w) {
        p = saved;
        return false;
    } else {
        return true;
    }
}

/**
 * @brief parseDouble : parse number [s]dddd[.dddd] or [s]d[.dddd][E|e][s][dddd] and store the result in x
 * @param p the string to parse, then the remaining string
 * @param x the float number found if any
 * @return true if a float number was found at the begin of p
 */
static bool parseDouble(const char*& p, double& x)
{
    double sign = 1.0;     // sign of the number
    double ipart = 0;      // integral part of the number
    double dpart = 0;      // decimal part of the number before division
    double dcoef = 1.0;    // division factor for the decimal part
    double expsign = 1.0;  // sign of the E|e part
    double expcoef = 0.0;  // multiplication factor of E|e part
    
    bool valid = false;    // true if the number contains at least one digit
    
    skipBlank(p);
    const char* saved = p;  // to restore position if we fail
    
    // Sign
    if (parseChar(p, '+')) {
        sign = 1.0;
    } else if (parseChar(p, '-')) {
        sign = -1.0;
    }
    
    // Integral part
    while (isdigit(*p)) {
        valid = true;
        ipart = ipart*10 + (*p - '0');
        p++;
    }
    
    // Possible decimal part
    if (parseChar(p, '.')) {
        while (isdigit(*p)) {
            valid = true;
            dpart = dpart*10 + (*p - '0');
            dcoef *= 10.0;
            p++;
        }
    }
    
    // Possible E|e part
    if (parseChar(p, 'E') || parseChar(p, 'e')) {
        if (parseChar(p, '+')) {
            expsign = 1.0;
        } else if (parseChar(p, '-')) {
            expsign = -1.0;
        }
        while (isdigit(*p)) {
            expcoef = expcoef*10 + (*p - '0');
            p++;
        }
    }
    
    if (valid)  {
        x = (sign*(ipart + dpart/dcoef)) * std::pow(10.0, expcoef*expsign);
    } else {
        p = saved;
    }
    return valid;
}

/**
 * @brief parseString, parse an arbitrary quoted string q...q and store the result in s
 * @param p the string to parse, then the remaining string
 * @param quote the character used to quote the string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseString(const char*& p, char quote, std::string& s)
{
    std::string str;
    skipBlank(p);
    
    const char* saved = p;  // to restore position if we fail
    if (*p++ == quote) {
        while ((*p != 0) && (*p != quote)) {
            str += *p++;
        }
        if (*p++ == quote) {
            s = str;
            return true;
        }
    }
    p = saved;
    return false;
}

/**
 * @brief parseSQString, parse a single quoted string '...' and store the result in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseSQString(const char*& p, std::string& s)
{
    return parseString(p, '\'', s);
}

/**
 * @brief parseDQString, parse a double quoted string "..." and store the result in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseDQString(const char*& p, std::string& s)
{
    return parseString(p, '"', s);
}

// ---------------------------------------------------------------------
//
//                          IMPLEMENTATION
// 
// ---------------------------------------------------------------------

/**
 * @brief parseMenuItem, parse a menu item ...'low':440.0...
 * @param p the string to parse, then the remaining string
 * @param name the name found
 * @param value the value found
 * @return true if a nemu item was found
 */
static bool parseMenuItem(const char*& p, std::string& name, double& value)
{
    const char* saved = p;  // to restore position if we fail
    if (parseSQString(p, name) && parseChar(p, ':') && parseDouble(p, value)) {
        return true;
    } else {
        p = saved;
        return false;
    }
}

static bool parseMenuItem2(const char*& p, std::string& name)
{
    const char* saved = p;  // to restore position if we fail
    // single quoted
    if (parseSQString(p, name)) {
        return true;
    } else {
        p = saved;
        return false;
    }
}

/**
 * @brief parseMenuList, parse a menu list {'low' : 440.0; 'mid' : 880.0; 'hi' : 1760.0}...
 * @param p the string to parse, then the remaining string
 * @param names the vector of names found
 * @param values the vector of values found
 * @return true if a menu list was found
 */
static bool parseMenuList(const char*& p, std::vector<std::string>& names, std::vector<double>& values)
{
    std::vector<std::string> tmpnames;
    std::vector<double> tmpvalues;
    const char* saved = p; // to restore position if we fail

    if (parseChar(p, '{')) {
        do {
            std::string n;
            double v;
            if (parseMenuItem(p, n, v)) {
                tmpnames.push_back(n);
                tmpvalues.push_back(v);
            } else {
                p = saved;
                return false;
            }
        } while (parseChar(p, ';'));
        if (parseChar(p, '}')) {
            // we suceeded
            names = tmpnames;
            values = tmpvalues;
            return true;
        }
    }
    p = saved;
    return false;
}

static bool parseMenuList2(const char*& p, std::vector<std::string>& names, bool debug)
{
    std::vector<std::string> tmpnames;
    const char* saved = p;  // to restore position if we fail
    
    if (parseChar(p, '{')) {
        do {
            std::string n;
            if (parseMenuItem2(p, n)) {
                tmpnames.push_back(n);
            } else {
                goto error;
            }
        } while (parseChar(p, ';'));
        if (parseChar(p, '}')) {
            // we suceeded
            names = tmpnames;
            return true;
        }
    }
    
error:
    if (debug) { fprintf(stderr, "parseMenuList2 : (%s) is not a valid list !\n", p); }
    p = saved;
    return false;
}

/// ---------------------------------------------------------------------
// Parse list of strings
/// ---------------------------------------------------------------------
static bool parseList(const char*& p, std::vector<std::string>& items)
{
    const char* saved = p;  // to restore position if we fail
    if (parseChar(p, '[')) {
        do {
            std::string item;
            if (!parseDQString(p, item)) {
                p = saved;
                return false;
            }
            items.push_back(item);
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

static bool parseMetaData(const char*& p, std::map<std::string, std::string>& metadatas)
{
    const char* saved = p; // to restore position if we fail
    std::string metaKey, metaValue;
    if (parseChar(p, ':') && parseChar(p, '[')) {
        do { 
            if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') && parseDQString(p, metaValue) && parseChar(p, '}')) {
                metadatas[metaKey] = metaValue;
            }
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

static bool parseItemMetaData(const char*& p, std::vector<std::pair<std::string, std::string> >& metadatas)
{
    const char* saved = p; // to restore position if we fail
    std::string metaKey, metaValue;
    if (parseChar(p, ':') && parseChar(p, '[')) {
        do { 
            if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') && parseDQString(p, metaValue) && parseChar(p, '}')) {
                metadatas.push_back(std::make_pair(metaKey, metaValue));
            }
        } while (tryChar(p, ','));
        return parseChar(p, ']');
    } else {
        p = saved;
        return false;
    }
}

// ---------------------------------------------------------------------
// Parse metadatas of the interface:
// "name" : "...", "inputs" : "...", "outputs" : "...", ...
// and store the result as key/value
/// ---------------------------------------------------------------------
static bool parseGlobalMetaData(const char*& p, std::string& key, std::string& value, double& dbl, std::map<std::string, std::string>& metadatas, std::vector<std::string>& items)
{
    const char* saved = p; // to restore position if we fail
    if (parseDQString(p, key)) {
        if (key == "meta") {
            return parseMetaData(p, metadatas);
        } else {
            return parseChar(p, ':') && (parseDQString(p, value) || parseList(p, items) || parseDouble(p, dbl));
        }
    } else {
        p = saved;
        return false;
    }
}

// ---------------------------------------------------------------------
// Parse gui:
// "type" : "...", "label" : "...", "address" : "...", ...
// and store the result in uiItems Vector
/// ---------------------------------------------------------------------
static bool parseUI(const char*& p, std::vector<itemInfo>& uiItems, int& numItems)
{
    const char* saved = p; // to restore position if we fail
    if (parseChar(p, '{')) {
   
        std::string label;
        std::string value;
        double dbl = 0;
        
        do {
            if (parseDQString(p, label)) {
                if (label == "type") {
                    if (uiItems.size() != 0) {
                        numItems++;
                    }
                    if (parseChar(p, ':') && parseDQString(p, value)) {   
                        itemInfo item;
                        item.type = value;
                        uiItems.push_back(item);
                    }
                }
                
                else if (label == "label") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].label = value;
                    }
                }
                
                else if (label == "varname") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].varname = value;
                    }
                }
                
                else if (label == "shortname") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].shortname = value;
                    }
                }
                
                else if (label == "address") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].address = value;
                    }
                }
                
                else if (label == "url") {
                    if (parseChar(p, ':') && parseDQString(p, value)) {
                        uiItems[numItems].url = value;
                    }
                }
                
                else if (label == "index") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].index = int(dbl);
                    }
                }
                
                else if (label == "meta") {
                    if (!parseItemMetaData(p, uiItems[numItems].meta)) {
                        return false;
                    }
                }
                
                else if (label == "init") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].init = dbl;
                    }
                }
                
                else if (label == "min") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].fmin = dbl;
                    }
                }
                
                else if (label == "max") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].fmax = dbl;
                    }
                }
                
                else if (label == "step") {
                    if (parseChar(p, ':') && parseDouble(p, dbl)) {
                        uiItems[numItems].step = dbl;
                    }
                }
                
                else if (label == "items") {
                    if (parseChar(p, ':') && parseChar(p, '[')) {
                        do {
                            if (!parseUI(p, uiItems, numItems)) {
                                p = saved;
                                return false;
                            }
                        } while (tryChar(p, ','));
                        if (parseChar(p, ']')) {
                            itemInfo item;
                            item.type = "close";
                            uiItems.push_back(item);
                            numItems++;
                        }
                    }
            
                } else {
                    fprintf(stderr, "Parse error unknown : %s \n", label.c_str());
                    assert(false);
                }
            } else {
                p = saved;
                return false;
            }
            
        } while (tryChar(p, ','));
    
        return parseChar(p, '}');
    } else {
        return true; // "items": [] is valid
    }
}

// ---------------------------------------------------------------------
// Parse full JSON record describing a JSON/Faust interface :
// {"metadatas": "...", "ui": [{ "type": "...", "label": "...", "items": [...], "address": "...","init": "...", "min": "...", "max": "...","step": "..."}]}
//
// and store the result in map Metadatas and vector containing the items of the interface. Returns true if parsing was successfull.
/// ---------------------------------------------------------------------
static bool parseJson(const char*& p,
                      std::map<std::string, std::pair<std::string, double> >& metaDatas0,
                      std::map<std::string, std::string>& metaDatas1,
                      std::map<std::string, std::vector<std::string> >& metaDatas2,
                      std::vector<itemInfo>& uiItems)
{
    parseChar(p, '{');
    
    do {
        std::string key;
        std::string value;
        double dbl = 0;
        std::vector<std::string> items;
        if (parseGlobalMetaData(p, key, value, dbl, metaDatas1, items)) {
            if (key != "meta") {
                // keep "name", "inputs", "outputs" key/value pairs
                if (items.size() > 0) {
                    metaDatas2[key] = items;
                    items.clear();
                } else if (value != "") {
                    metaDatas0[key].first = value;
                } else {
                    metaDatas0[key].second = dbl;
                }
            }
        } else if (key == "ui") {
            int numItems = 0;
            parseChar(p, '[') && parseUI(p, uiItems, numItems);
        }
    } while (tryChar(p, ','));
    
    return parseChar(p, '}');
}

#endif // SIMPLEPARSER_H
/**************************  END  SimpleParser.h **************************/

static bool startWith(const std::string& str, const std::string& prefix)
{
    return (str.substr(0, prefix.size()) == prefix);
}

/**
 * Convert a dB value into a scale between 0 and 1 (following IEC standard ?)
 */
static FAUSTFLOAT dB2Scale(FAUSTFLOAT dB)
{
    FAUSTFLOAT scale = FAUSTFLOAT(1.0);
    
    /*if (dB < -70.0f)
     scale = 0.0f;
     else*/
    if (dB < FAUSTFLOAT(-60.0))
        scale = (dB + FAUSTFLOAT(70.0)) * FAUSTFLOAT(0.0025);
    else if (dB < FAUSTFLOAT(-50.0))
        scale = (dB + FAUSTFLOAT(60.0)) * FAUSTFLOAT(0.005) + FAUSTFLOAT(0.025);
    else if (dB < FAUSTFLOAT(-40.0))
        scale = (dB + FAUSTFLOAT(50.0)) * FAUSTFLOAT(0.0075) + FAUSTFLOAT(0.075);
    else if (dB < FAUSTFLOAT(-30.0))
        scale = (dB + FAUSTFLOAT(40.0)) * FAUSTFLOAT(0.015) + FAUSTFLOAT(0.15);
    else if (dB < FAUSTFLOAT(-20.0))
        scale = (dB + FAUSTFLOAT(30.0)) * FAUSTFLOAT(0.02) + FAUSTFLOAT(0.3);
    else if (dB < FAUSTFLOAT(-0.001) || dB > FAUSTFLOAT(0.001))  /* if (dB < 0.0) */
        scale = (dB + FAUSTFLOAT(20.0)) * FAUSTFLOAT(0.025) + FAUSTFLOAT(0.5);
    
    return scale;
}

/*******************************************************************************
 * MetaDataUI : Common class for MetaData handling
 ******************************************************************************/

//============================= BEGIN GROUP LABEL METADATA===========================
// Unlike widget's label, metadata inside group's label are not extracted directly by
// the Faust compiler. Therefore they must be extracted within the architecture file
//-----------------------------------------------------------------------------------

class MetaDataUI {
    
    protected:
        
        std::string                         fGroupTooltip;
        std::map<FAUSTFLOAT*, FAUSTFLOAT>   fGuiSize;            // map widget zone with widget size coef
        std::map<FAUSTFLOAT*, std::string>  fTooltip;            // map widget zone with tooltip strings
        std::map<FAUSTFLOAT*, std::string>  fUnit;               // map widget zone to unit string (i.e. "dB")
        std::map<FAUSTFLOAT*, std::string>  fRadioDescription;   // map zone to {'low':440; ...; 'hi':1000.0}
        std::map<FAUSTFLOAT*, std::string>  fMenuDescription;    // map zone to {'low':440; ...; 'hi':1000.0}
        std::set<FAUSTFLOAT*>               fKnobSet;            // set of widget zone to be knobs
        std::set<FAUSTFLOAT*>               fLedSet;             // set of widget zone to be LEDs
        std::set<FAUSTFLOAT*>               fNumSet;             // set of widget zone to be numerical bargraphs
        std::set<FAUSTFLOAT*>               fLogSet;             // set of widget zone having a log UI scale
        std::set<FAUSTFLOAT*>               fExpSet;             // set of widget zone having an exp UI scale
        std::set<FAUSTFLOAT*>               fHiddenSet;          // set of hidden widget zone
        
        void clearMetadata()
        {
            fGuiSize.clear();
            fTooltip.clear();
            fUnit.clear();
            fRadioDescription.clear();
            fMenuDescription.clear();
            fKnobSet.clear();
            fLedSet.clear();
            fNumSet.clear();
            fLogSet.clear();
            fExpSet.clear();
            fHiddenSet.clear();
            fGroupTooltip = "";
        }
        
        /**
         * rmWhiteSpaces(): Remove the leading and trailing white spaces of a string
         * (but not those in the middle of the string)
         */
        static std::string rmWhiteSpaces(const std::string& s)
        {
            size_t i = s.find_first_not_of(" \t");
            size_t j = s.find_last_not_of(" \t");
            if ((i != std::string::npos) && (j != std::string::npos)) {
                return s.substr(i, 1+j-i);
            } else {
                return "";
            }
        }
        
        /**
         * Format tooltip string by replacing some white spaces by
         * return characters so that line width doesn't exceed n.
         * Limitation : long words exceeding n are not cut.
         */
        std::string formatTooltip(int n, const std::string& tt)
        {
            std::string ss = tt;  // ss string we are going to format
            int lws = 0;          // last white space encountered
            int lri = 0;          // last return inserted
            for (int i = 0; i < (int)tt.size(); i++) {
                if (tt[i] == ' ') lws = i;
                if (((i-lri) >= n) && (lws > lri)) {
                    // insert return here
                    ss[lws] = '\n';
                    lri = lws;
                }
            }
            return ss;
        }
    
        std::vector<std::pair<std::string, double> > parseOptionList(const std::string& desc) const
        {
            const char* mdescr = desc.c_str();
            std::vector<std::string> names;
            std::vector<double> values;
            parseMenuList(mdescr, names, values);
            std::vector<std::pair<std::string, double> > opts;
            for (size_t i = 0; i < names.size(); i++) {
                opts.push_back({names[i], values[i]});
            }
            return opts;
        }
        
    public:
        
        virtual ~MetaDataUI()
        {}
        
        enum Scale {
            kLin,
            kLog,
            kExp
        };
        
        Scale getScale(FAUSTFLOAT* zone)
        {
            if (fLogSet.count(zone) > 0) return kLog;
            if (fExpSet.count(zone) > 0) return kExp;
            return kLin;
        }
        
        bool isKnob(FAUSTFLOAT* zone)
        {
            return fKnobSet.count(zone) > 0;
        }
        
        bool isRadio(FAUSTFLOAT* zone)
        {
            return fRadioDescription.count(zone) > 0;
        }
        
        bool isMenu(FAUSTFLOAT* zone)
        {
            return fMenuDescription.count(zone) > 0;
        }
        
        bool isLed(FAUSTFLOAT* zone)
        {
            return fLedSet.count(zone) > 0;
        }
        
        bool isNumerical(FAUSTFLOAT* zone)
        {
            return fNumSet.count(zone) > 0;
        }
        
        bool isHidden(FAUSTFLOAT* zone)
        {
            return fHiddenSet.count(zone) > 0;
        }

        std::string getTooltip(FAUSTFLOAT* zone) const
        {
            auto it = fTooltip.find(zone);
            if (it != fTooltip.end()) {
                return it->second;
            }
            return fGroupTooltip;
        }

        std::string getUnit(FAUSTFLOAT* zone) const
        {
            auto it = fUnit.find(zone);
            if (it != fUnit.end()) {
                return it->second;
            }
            return "";
        }

        std::vector<std::pair<std::string, double> > getRadioDescription(FAUSTFLOAT* zone) const
        {
            auto it = fRadioDescription.find(zone);
            if (it == fRadioDescription.end()) {
                return {};
            }
            return parseOptionList(it->second);
        }

        std::vector<std::pair<std::string, double> > getMenuDescription(FAUSTFLOAT* zone) const
        {
            auto it = fMenuDescription.find(zone);
            if (it == fMenuDescription.end()) {
                return {};
            }
            return parseOptionList(it->second);
        }
        
        /**
         * Extracts metadata from a label : 'vol [unit: dB]' -> 'vol' + metadata(unit=dB)
         */
        static void extractMetadata(const std::string& fulllabel, std::string& label, std::map<std::string, std::string>& metadata)
        {
            enum {kLabel, kEscape1, kEscape2, kEscape3, kKey, kValue};
            int state = kLabel; int deep = 0;
            std::string key, value;
            
            for (unsigned int i = 0; i < fulllabel.size(); i++) {
                char c = fulllabel[i];
                switch (state) {
                    case kLabel :
                        assert(deep == 0);
                        switch (c) {
                            case '\\' : state = kEscape1; break;
                            case '[' : state = kKey; deep++; break;
                            default : label += c;
                        }
                        break;
                        
                    case kEscape1:
                        label += c;
                        state = kLabel;
                        break;
                        
                    case kEscape2:
                        key += c;
                        state = kKey;
                        break;
                        
                    case kEscape3:
                        value += c;
                        state = kValue;
                        break;
                        
                    case kKey:
                        assert(deep > 0);
                        switch (c) {
                            case '\\':
                                state = kEscape2;
                                break;
                                
                            case '[':
                                deep++;
                                key += c;
                                break;
                                
                            case ':':
                                if (deep == 1) {
                                    state = kValue;
                                } else {
                                    key += c;
                                }
                                break;
                            case ']':
                                deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)] = "";
                                    state = kLabel;
                                    key = "";
                                    value = "";
                                } else {
                                    key += c;
                                }
                                break;
                            default : key += c;
                        }
                        break;
                        
                    case kValue:
                        assert(deep > 0);
                        switch (c) {
                            case '\\':
                                state = kEscape3;
                                break;
                                
                            case '[':
                                deep++;
                                value += c;
                                break;
                                
                            case ']':
                                deep--;
                                if (deep < 1) {
                                    metadata[rmWhiteSpaces(key)] = rmWhiteSpaces(value);
                                    state = kLabel;
                                    key = "";
                                    value = "";
                                } else {
                                    value += c;
                                }
                                break;
                            default : value += c;
                        }
                        break;
                        
                    default:
                        fprintf(stderr, "ERROR unrecognized state %d\n", state);
                }
            }
            label = rmWhiteSpaces(label);
        }
        
        /**
         * Analyses the widget zone metadata declarations and takes appropriate actions.
         */
        void declare(FAUSTFLOAT* zone, const char* key, const char* value)
        {
            if (zone == 0) {
                // special zone 0 means group metadata
                if (strcmp(key, "tooltip") == 0) {
                    // only group tooltip are currently implemented
                    fGroupTooltip = formatTooltip(30, value);
                } else if ((strcmp(key, "hidden") == 0) && (strcmp(value, "1") == 0)) {
                    fHiddenSet.insert(zone);
                }
            } else {
                if (strcmp(key, "size") == 0) {
                    fGuiSize[zone] = atof(value);
                }
                else if (strcmp(key, "tooltip") == 0) {
                    fTooltip[zone] = formatTooltip(30, value);
                }
                else if (strcmp(key, "unit") == 0) {
                    fUnit[zone] = value;
                }
                else if ((strcmp(key, "hidden") == 0) && (strcmp(value, "1") == 0)) {
                    fHiddenSet.insert(zone);
                }
                else if (strcmp(key, "scale") == 0) {
                    if (strcmp(value, "log") == 0) {
                        fLogSet.insert(zone);
                    } else if (strcmp(value, "exp") == 0) {
                        fExpSet.insert(zone);
                    }
                }
                else if (strcmp(key, "style") == 0) {
                    if (strcmp(value, "knob") == 0) {
                        fKnobSet.insert(zone);
                    } else if (strcmp(value, "led") == 0) {
                        fLedSet.insert(zone);
                    } else if (strcmp(value, "numerical") == 0) {
                        fNumSet.insert(zone);
                    } else {
                        const char* p = value;
                        if (parseWord(p, "radio")) {
                            fRadioDescription[zone] = std::string(p);
                        } else if (parseWord(p, "menu")) {
                            fMenuDescription[zone] = std::string(p);
                        }
                    }
                }
            }
        }
    
};

#endif
/**************************  END  MetaDataUI.h **************************/
/************************** BEGIN ring-buffer.h **************************/
/*
  Copyright (C) 2000 Paul Davis
  Copyright (C) 2003 Rohan Drape
  Copyright (C) 2016 GRAME (renaming for internal use)

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

  ISO/POSIX C version of Paul Davis's lock free ringbuffer C++ code.
  This is safe for the case of one read thread and one write thread.
*/

#ifndef __ring_buffer__
#define __ring_buffer__

#include <stdlib.h>
#include <string.h>

#ifdef WIN32
# pragma warning (disable: 4334)
#else
# pragma GCC diagnostic ignored "-Wunused-function"
#endif

typedef struct {
    char *buf;
    size_t len;
}
ringbuffer_data_t;

typedef struct {
    char *buf;
    volatile size_t write_ptr;
    volatile size_t read_ptr;
    size_t	size;
    size_t	size_mask;
    int	mlocked;
}
ringbuffer_t;

static ringbuffer_t *ringbuffer_create(size_t sz);
static void ringbuffer_free(ringbuffer_t *rb);
static void ringbuffer_get_read_vector(const ringbuffer_t *rb,
                                         ringbuffer_data_t *vec);
static void ringbuffer_get_write_vector(const ringbuffer_t *rb,
                                          ringbuffer_data_t *vec);
static size_t ringbuffer_read(ringbuffer_t *rb, char *dest, size_t cnt);
static size_t ringbuffer_peek(ringbuffer_t *rb, char *dest, size_t cnt);
static void ringbuffer_read_advance(ringbuffer_t *rb, size_t cnt);
static size_t ringbuffer_read_space(const ringbuffer_t *rb);
static int ringbuffer_mlock(ringbuffer_t *rb);
static void ringbuffer_reset(ringbuffer_t *rb);
static void ringbuffer_reset_size (ringbuffer_t * rb, size_t sz);
static size_t ringbuffer_write(ringbuffer_t *rb, const char *src,
                                 size_t cnt);
static void ringbuffer_write_advance(ringbuffer_t *rb, size_t cnt);
static size_t ringbuffer_write_space(const ringbuffer_t *rb);

/* Create a new ringbuffer to hold at least `sz' bytes of data. The
   actual buffer size is rounded up to the next power of two. */

static ringbuffer_t *
ringbuffer_create (size_t sz)
{
	size_t power_of_two;
	ringbuffer_t *rb;

	if ((rb = (ringbuffer_t *) malloc (sizeof (ringbuffer_t))) == NULL) {
		return NULL;
	}

	for (power_of_two = 1u; 1u << power_of_two < sz; power_of_two++);

	rb->size = 1u << power_of_two;
	rb->size_mask = rb->size;
	rb->size_mask -= 1;
	rb->write_ptr = 0;
	rb->read_ptr = 0;
	if ((rb->buf = (char *) malloc (rb->size)) == NULL) {
		free (rb);
		return NULL;
	}
	rb->mlocked = 0;

	return rb;
}

/* Free all data associated with the ringbuffer `rb'. */

static void
ringbuffer_free (ringbuffer_t * rb)
{
#ifdef USE_MLOCK
	if (rb->mlocked) {
		munlock (rb->buf, rb->size);
	}
#endif /* USE_MLOCK */
	free (rb->buf);
	free (rb);
}

/* Lock the data block of `rb' using the system call 'mlock'.  */

static int
ringbuffer_mlock (ringbuffer_t * rb)
{
#ifdef USE_MLOCK
	if (mlock (rb->buf, rb->size)) {
		return -1;
	}
#endif /* USE_MLOCK */
	rb->mlocked = 1;
	return 0;
}

/* Reset the read and write pointers to zero. This is not thread
   safe. */

static void
ringbuffer_reset (ringbuffer_t * rb)
{
	rb->read_ptr = 0;
	rb->write_ptr = 0;
    memset(rb->buf, 0, rb->size);
}

/* Reset the read and write pointers to zero. This is not thread
   safe. */

static void
ringbuffer_reset_size (ringbuffer_t * rb, size_t sz)
{
    rb->size = sz;
    rb->size_mask = rb->size;
    rb->size_mask -= 1;
    rb->read_ptr = 0;
    rb->write_ptr = 0;
}

/* Return the number of bytes available for reading. This is the
   number of bytes in front of the read pointer and behind the write
   pointer.  */

static size_t
ringbuffer_read_space (const ringbuffer_t * rb)
{
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		return w - r;
	} else {
		return (w - r + rb->size) & rb->size_mask;
	}
}

/* Return the number of bytes available for writing. This is the
   number of bytes in front of the write pointer and behind the read
   pointer.  */

static size_t
ringbuffer_write_space (const ringbuffer_t * rb)
{
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		return ((r - w + rb->size) & rb->size_mask) - 1;
	} else if (w < r) {
		return (r - w) - 1;
	} else {
		return rb->size - 1;
	}
}

/* The copying data reader. Copy at most `cnt' bytes from `rb' to
   `dest'.  Returns the actual number of bytes copied. */

static size_t
ringbuffer_read (ringbuffer_t * rb, char *dest, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_read;
	size_t n1, n2;

	if ((free_cnt = ringbuffer_read_space (rb)) == 0) {
		return 0;
	}

	to_read = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = rb->read_ptr + to_read;

	if (cnt2 > rb->size) {
		n1 = rb->size - rb->read_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_read;
		n2 = 0;
	}

	memcpy (dest, &(rb->buf[rb->read_ptr]), n1);
	rb->read_ptr = (rb->read_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (dest + n1, &(rb->buf[rb->read_ptr]), n2);
		rb->read_ptr = (rb->read_ptr + n2) & rb->size_mask;
	}

	return to_read;
}

/* The copying data reader w/o read pointer advance. Copy at most
   `cnt' bytes from `rb' to `dest'.  Returns the actual number of bytes
   copied. */

static size_t
ringbuffer_peek (ringbuffer_t * rb, char *dest, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_read;
	size_t n1, n2;
	size_t tmp_read_ptr;

	tmp_read_ptr = rb->read_ptr;

	if ((free_cnt = ringbuffer_read_space (rb)) == 0) {
		return 0;
	}

	to_read = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = tmp_read_ptr + to_read;

	if (cnt2 > rb->size) {
		n1 = rb->size - tmp_read_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_read;
		n2 = 0;
	}

	memcpy (dest, &(rb->buf[tmp_read_ptr]), n1);
	tmp_read_ptr = (tmp_read_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (dest + n1, &(rb->buf[tmp_read_ptr]), n2);
	}

	return to_read;
}

/* The copying data writer. Copy at most `cnt' bytes to `rb' from
   `src'.  Returns the actual number of bytes copied. */

static size_t
ringbuffer_write (ringbuffer_t * rb, const char *src, size_t cnt)
{
	size_t free_cnt;
	size_t cnt2;
	size_t to_write;
	size_t n1, n2;

	if ((free_cnt = ringbuffer_write_space (rb)) == 0) {
		return 0;
	}

	to_write = cnt > free_cnt ? free_cnt : cnt;

	cnt2 = rb->write_ptr + to_write;

	if (cnt2 > rb->size) {
		n1 = rb->size - rb->write_ptr;
		n2 = cnt2 & rb->size_mask;
	} else {
		n1 = to_write;
		n2 = 0;
	}

	memcpy (&(rb->buf[rb->write_ptr]), src, n1);
	rb->write_ptr = (rb->write_ptr + n1) & rb->size_mask;

	if (n2) {
		memcpy (&(rb->buf[rb->write_ptr]), src + n1, n2);
		rb->write_ptr = (rb->write_ptr + n2) & rb->size_mask;
	}

	return to_write;
}

/* Advance the read pointer `cnt' places. */

static void
ringbuffer_read_advance (ringbuffer_t * rb, size_t cnt)
{
	size_t tmp = (rb->read_ptr + cnt) & rb->size_mask;
	rb->read_ptr = tmp;
}

/* Advance the write pointer `cnt' places. */

static void
ringbuffer_write_advance (ringbuffer_t * rb, size_t cnt)
{
	size_t tmp = (rb->write_ptr + cnt) & rb->size_mask;
	rb->write_ptr = tmp;
}

/* The non-copying data reader. `vec' is an array of two places. Set
   the values at `vec' to hold the current readable data at `rb'. If
   the readable data is in one segment the second segment has zero
   length. */

static void
ringbuffer_get_read_vector (const ringbuffer_t * rb,
				 ringbuffer_data_t * vec)
{
	size_t free_cnt;
	size_t cnt2;
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		free_cnt = w - r;
	} else {
		free_cnt = (w - r + rb->size) & rb->size_mask;
	}

	cnt2 = r + free_cnt;

	if (cnt2 > rb->size) {

		/* Two part vector: the rest of the buffer after the current write
		   ptr, plus some from the start of the buffer. */

		vec[0].buf = &(rb->buf[r]);
		vec[0].len = rb->size - r;
		vec[1].buf = rb->buf;
		vec[1].len = cnt2 & rb->size_mask;

	} else {

		/* Single part vector: just the rest of the buffer */

		vec[0].buf = &(rb->buf[r]);
		vec[0].len = free_cnt;
		vec[1].len = 0;
	}
}

/* The non-copying data writer. `vec' is an array of two places. Set
   the values at `vec' to hold the current writeable data at `rb'. If
   the writeable data is in one segment the second segment has zero
   length. */

static void
ringbuffer_get_write_vector (const ringbuffer_t * rb,
				  ringbuffer_data_t * vec)
{
	size_t free_cnt;
	size_t cnt2;
	size_t w, r;

	w = rb->write_ptr;
	r = rb->read_ptr;

	if (w > r) {
		free_cnt = ((r - w + rb->size) & rb->size_mask) - 1;
	} else if (w < r) {
		free_cnt = (r - w) - 1;
	} else {
		free_cnt = rb->size - 1;
	}

	cnt2 = w + free_cnt;

	if (cnt2 > rb->size) {

		/* Two part vector: the rest of the buffer after the current write
		   ptr, plus some from the start of the buffer. */

		vec[0].buf = &(rb->buf[w]);
		vec[0].len = rb->size - w;
		vec[1].buf = rb->buf;
		vec[1].len = cnt2 & rb->size_mask;
	} else {
		vec[0].buf = &(rb->buf[w]);
		vec[0].len = free_cnt;
		vec[1].len = 0;
	}
}

#endif // __ring_buffer__
/**************************  END  ring-buffer.h **************************/

/*******************************************************************************
 * GUI : Abstract Graphic User Interface
 * Provides additional mechanisms to synchronize widgets and zones. Widgets
 * should both reflect the value of a zone and allow to change this value.
 ******************************************************************************/

class uiItem;
class GUI;
struct clist;

typedef void (*uiCallback)(FAUSTFLOAT val, void* data);

/**
 * Base class for uiTypedItem: memory zones that can be grouped and synchronized, using an internal cache.
 */
struct uiItemBase
{
    
    uiItemBase(GUI* ui, FAUSTFLOAT* zone)
    {
        assert(ui);
        assert(zone);
    }
    
    virtual ~uiItemBase()
    {}
    
    /**
     * This method will be called when the value changes externally,
     * and will signal the new value to all linked uItem
     * when the value is different from the cached one.
     *
     * @param v - the new value
     */
    virtual void modifyZone(FAUSTFLOAT v) = 0;
    
    /**
     * This method will be called when the value changes externally,
     * and will signal the new value to all linked uItem
     * when the value is different from the cached one.
     *
     * @param date - the timestamp of the received value in usec
     * @param v - the new value
     */
    virtual void modifyZone(double date, FAUSTFLOAT v) {}
    
    /**
     * This method is called by the synchronisation mecanism and is expected
     * to 'reflect' the new value, by changing the Widget layout for instance,
     * or sending a message (OSC, MIDI...)
     */
    virtual void reflectZone() = 0;
    
    /**
     * Return the cached value.
     *
     * @return - the cached value
     */
    virtual double cache() = 0;
    
};

// Declared as 'static' to avoid code duplication at link time
static void deleteClist(clist* cl);

/**
 * A list containing all groupe uiItemBase objects.
 */
struct clist : public std::list<uiItemBase*>
{
    
    virtual ~clist()
    {
        deleteClist(this);
    }
        
};

static void createUiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data);

typedef std::map<FAUSTFLOAT*, clist*> zmap;

typedef std::map<FAUSTFLOAT*, ringbuffer_t*> ztimedmap;

class GUI : public UI
{
		
    private:
     
        static std::list<GUI*> fGuiList;
        zmap fZoneMap;
        bool fStopped;
    
     public:
            
        GUI():fStopped(false)
        {	
            fGuiList.push_back(this);
        }
        
        virtual ~GUI() 
        {   
            // delete all items
            for (const auto& it : fZoneMap) {
                delete it.second;
            }
            // suppress 'this' in static fGuiList
            fGuiList.remove(this);
        }

        // -- registerZone(z,c) : zone management
        
        void registerZone(FAUSTFLOAT* z, uiItemBase* c)
        {
            if (fZoneMap.find(z) == fZoneMap.end()) fZoneMap[z] = new clist();
            fZoneMap[z]->push_back(c);
        }
    
        void updateZone(FAUSTFLOAT* z)
        {
            FAUSTFLOAT v = *z;
            clist* cl = fZoneMap[z];
            for (const auto& c : *cl) {
                if (c->cache() != v) c->reflectZone();
            }
        }
    
        void updateAllZones()
        {
            for (const auto& m : fZoneMap) {
                updateZone(m.first);
            }
        }
    
        // To be implemented when a single global update is needed per updateAllGuis refresh
        virtual void updateAll()
        {}
        
        static void updateAllGuis()
        {
            for (const auto& g : fGuiList) {
                g->updateAllZones();
                g->updateAll();
            }
        }
    
        void addCallback(FAUSTFLOAT* zone, uiCallback foo, void* data)
        {
            createUiCallbackItem(this, zone, foo, data);
        }

        // Start event or message processing
        virtual bool run() { return false; };
        // Stop event or message processing
        virtual void stop() { fStopped = true; }
        bool stopped() { return fStopped; }
    
        // -- widget's layouts
        
        virtual void openTabBox(const char* label) {}
        virtual void openHorizontalBox(const char* label) {}
        virtual void openVerticalBox(const char* label) {}
        virtual void closeBox() {}
        
        // -- active widgets
        
        virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
    
        // -- passive widgets
        
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) {}
    
        // -- metadata declarations

        virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
    
        // Static global for timed zones, shared between all UI that will set timed values
        static ztimedmap gTimedZoneMap;

};

/**
 * User Interface Item: abstract definition.
 */
template <typename REAL>
class uiTypedItemReal : public uiItemBase
{
    protected:
        
        GUI* fGUI;
        REAL* fZone;
        REAL fCache;
        
        uiTypedItemReal(GUI* ui, REAL* zone):uiItemBase(ui, static_cast<FAUSTFLOAT*>(zone)),
        fGUI(ui), fZone(zone), fCache(REAL(-123456.654321))
        {
            ui->registerZone(zone, this);
        }
        
    public:
        
        virtual ~uiTypedItemReal()
        {}
    
        void modifyZone(REAL v)
        {
            fCache = v;
            if (*fZone != v) {
                *fZone = v;
                fGUI->updateZone(fZone);
            }
        }
    
        double cache() { return fCache; }
    
};

class uiItem : public uiTypedItemReal<FAUSTFLOAT> {
    
    protected:
    
        uiItem(GUI* ui, FAUSTFLOAT* zone):uiTypedItemReal<FAUSTFLOAT>(ui, zone)
        {}

    public:

        virtual ~uiItem() 
        {}

		void modifyZone(FAUSTFLOAT v)
		{
			fCache = v;
			if (*fZone != v) {
				*fZone = v;
				fGUI->updateZone(fZone);
			}
		}

};

/**
 * Base class for items with a value converter.
 */
struct uiConverter {
    
    ValueConverter* fConverter;
    
    uiConverter(MetaDataUI::Scale scale, FAUSTFLOAT umin, FAUSTFLOAT umax, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
    {
        // Select appropriate converter according to scale mode
        if (scale == MetaDataUI::kLog) {
            fConverter = new LogValueConverter(umin, umax, fmin, fmax);
        } else if (scale == MetaDataUI::kExp) {
            fConverter = new ExpValueConverter(umin, umax, fmin, fmax);
        } else {
            fConverter = new LinearValueConverter(umin, umax, fmin, fmax);
        }
    }
    
    virtual ~uiConverter()
    {
        delete fConverter;
    }
};

/**
 * User Interface item owned (and so deleted) by external code.
 */
class uiOwnedItem : public uiItem {
    
    protected:
    
        uiOwnedItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {}
    
     public:
    
        virtual ~uiOwnedItem()
        {}
    
        virtual void reflectZone() {}
};

/**
 * Callback Item.
 */
class uiCallbackItem : public uiItem {
    
    protected:
    
        uiCallback fCallback;
        void* fData;
    
    public:
    
        uiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data)
        : uiItem(ui, zone), fCallback(foo), fData(data) {}
        
        virtual void reflectZone() 
        {		
            FAUSTFLOAT v = *fZone;
            fCache = v; 
            fCallback(v, fData);	
        }
};

/**
 *  For timestamped control.
 */
struct DatedControl {
    
    double fDate;
    FAUSTFLOAT fValue;
    
    DatedControl(double d = 0., FAUSTFLOAT v = FAUSTFLOAT(0)):fDate(d), fValue(v) {}
    
};

/**
 * Base class for timed items.
 */
class uiTimedItem : public uiItem
{
    
    protected:
        
        bool fDelete;
        
    public:
    
        using uiItem::modifyZone;
        
        uiTimedItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {
            if (GUI::gTimedZoneMap.find(fZone) == GUI::gTimedZoneMap.end()) {
                GUI::gTimedZoneMap[fZone] = ringbuffer_create(8192);
                fDelete = true;
            } else {
                fDelete = false;
            }
        }
        
        virtual ~uiTimedItem()
        {
            ztimedmap::iterator it;
            if (fDelete && ((it = GUI::gTimedZoneMap.find(fZone)) != GUI::gTimedZoneMap.end())) {
                ringbuffer_free((*it).second);
                GUI::gTimedZoneMap.erase(it);
            }
        }
        
        virtual void modifyZone(double date, FAUSTFLOAT v)
        {
            size_t res;
            DatedControl dated_val(date, v);
            if ((res = ringbuffer_write(GUI::gTimedZoneMap[fZone], (const char*)&dated_val, sizeof(DatedControl))) != sizeof(DatedControl)) {
                fprintf(stderr, "ringbuffer_write error DatedControl\n");
            }
        }
    
};

/**
 * Allows to group a set of zones.
 */
class uiGroupItem : public uiItem
{
    protected:
    
        std::vector<FAUSTFLOAT*> fZoneMap;

    public:
    
        uiGroupItem(GUI* ui, FAUSTFLOAT* zone):uiItem(ui, zone)
        {}
        virtual ~uiGroupItem() 
        {}
        
        virtual void reflectZone() 
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            
            // Update all zones of the same group
            for (const auto& it : fZoneMap) {
                *it = v;
            }
        }
        
        void addZone(FAUSTFLOAT* zone) { fZoneMap.push_back(zone); }

};

// Cannot be defined as method in the classes.

static void createUiCallbackItem(GUI* ui, FAUSTFLOAT* zone, uiCallback foo, void* data)
{
    new uiCallbackItem(ui, zone, foo, data);
}

static void deleteClist(clist* cl)
{
    for (const auto& it : *cl) {
        // This specific code is only used in JUCE context. TODO: use proper 'shared_ptr' based memory management.
    #if defined(JUCE_32BIT) || defined(JUCE_64BIT)
        uiOwnedItem* owned = dynamic_cast<uiOwnedItem*>(it);
        // owned items are deleted by external code
        if (!owned) {
            delete it;
        }
    #else
        delete it;
    #endif
    }
}

#endif
/**************************  END  GUI.h **************************/
/************************** BEGIN JSONUI.h *****************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_JSONUI_H
#define FAUST_JSONUI_H

#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>
#include <iterator>
#include <cstring>


/*******************************************************************************
 * JSONUI : Faust User Interface
 * This class produce a complete JSON description of the DSP instance.
 *
 * Since 'shortname' can only be computed when all paths have been created,
 * the fAllUI vector is progressively filled with partially built UI items,
 * which are finally created in the JSON(...) method.
 ******************************************************************************/

// Instruction complexity statistics
struct InstComplexity {
    
    int fLoad = 0;
    int fStore = 0;
    int fBinop = 0;
    int fMathop = 0;
    int fNumbers = 0;
    int fDeclare = 0;
    int fCast = 0;
    int fSelect = 0;
    int fLoop = 0;
    
    std::map<std::string, int> fFunctionSymbolTable;
    std::map<std::string, int> fBinopSymbolTable;
   
    InstComplexity operator+(const InstComplexity& icomp)
    {
        fLoad += icomp.fLoad;
        fStore += icomp.fStore;
        fBinop += icomp.fBinop;
        fMathop += icomp.fMathop;
        fNumbers += icomp.fNumbers;
        fDeclare += icomp.fDeclare;
        fCast += icomp.fCast;
        fSelect += icomp.fSelect;
        fLoop += icomp.fLoop;
        return *this;
    }
};

// DSP or field name, type, size, size-in-bytes, reads, writes
struct MemoryLayoutItem {
    std::string name;
    std::string type;
    int size;
    int size_bytes;
    int read;
    int write;
    
    static std::map<std::string, std::string> gStringType;
};

typedef std::vector<MemoryLayoutItem> MemoryLayoutType;
typedef std::map<std::string, int> PathTableType;

/*
    Build a JSON description of the DSP.
 */
template <typename REAL>
class FAUST_API JSONUIReal : public PathBuilder, public Meta, public UIReal<REAL> {

    protected:
    
        std::stringstream fUI;
        std::vector<std::string> fAllUI;
        std::stringstream fMeta;
        std::vector<std::pair <std::string, std::string> > fMetaAux;
        std::string fVersion;           // Compiler version
        std::string fCompileOptions;    // Compilation options
        std::vector<std::string> fLibraryList;
        std::vector<std::string> fIncludePathnames;
        std::string fName;
        std::string fFileName;
        std::string fExpandedCode;
        std::string fSHAKey;
        std::string fJSON;
        int fDSPSize;                   // In bytes
        PathTableType fPathTable;
        MemoryLayoutType fMemoryLayout;
        InstComplexity fIComp;
        bool fExtended;
    
        char fCloseUIPar;
        char fCloseMetaPar;
        int fTab;
    
        int fInputs, fOutputs, fSRIndex;
         
        void tab(int n, std::ostream& fout) noexcept
        {
            fout << '\n';
            while (n-- > 0) {
                fout << '\t';
            }
        }
    
        std::string flatten(const std::string& src) const
        {
            std::string dst;
            for (size_t i = 0; i < src.size(); i++) {
                switch (src[i]) {
                    case '\n':
                    case '\t':
                        break;
                    default:
                        dst += src[i];
                        break;
                }
            }
            return dst;
        }
    
        void addMeta(int tab_val, bool quote = true)
        {
            if (fMetaAux.size() > 0) {
                tab(tab_val, fUI); fUI << "\"meta\": [";
                std::string sep = "";
                for (size_t i = 0; i < fMetaAux.size(); i++) {
                    fUI << sep;
                    tab(tab_val + 1, fUI); fUI << "{ \"" << fMetaAux[i].first << "\": \"" << fMetaAux[i].second << "\" }";
                    sep = ",";
                }
                tab(tab_val, fUI); fUI << ((quote) ? "],": "]");
                fMetaAux.clear();
            }
        }
    
        int getAddressIndex(const std::string& path)
        {
            return (fPathTable.find(path) != fPathTable.end()) ? fPathTable[path] : -1;
        }
      
     public:
     
        JSONUIReal(const std::string& name,
                  const std::string& filename,
                  int inputs,
                  int outputs,
                  int sr_index,
                  const std::string& sha_key,
                  const std::string& dsp_code,
                  const std::string& version,
                  const std::string& compile_options,
                  const std::vector<std::string>& library_list,
                  const std::vector<std::string>& include_pathnames,
                  int size,
                  const PathTableType& path_table,
                  MemoryLayoutType memory_layout,
                  InstComplexity inst_comp)
        {
            init(name, filename, inputs, outputs, sr_index, sha_key, dsp_code, version, compile_options, library_list, include_pathnames, size, path_table, memory_layout, inst_comp);
        }

        JSONUIReal(const std::string& name, const std::string& filename, int inputs, int outputs)
        {
            init(name, filename, inputs, outputs, -1, "", "", "", "", std::vector<std::string>(), std::vector<std::string>(), -1, PathTableType(), MemoryLayoutType(), InstComplexity());
        }

        JSONUIReal(int inputs, int outputs)
        {
            init("", "", inputs, outputs, -1, "", "","", "", std::vector<std::string>(), std::vector<std::string>(), -1, PathTableType(), MemoryLayoutType(), InstComplexity());
        }
        
        JSONUIReal()
        {
            init("", "", -1, -1, -1, "", "", "", "", std::vector<std::string>(), std::vector<std::string>(), -1, PathTableType(), MemoryLayoutType(), InstComplexity());
        }
 
        virtual ~JSONUIReal() {}
        
        void setInputs(int inputs) { fInputs = inputs; }
        void setOutputs(int outputs) { fOutputs = outputs; }
    
        void setSRIndex(int sr_index) { fSRIndex = sr_index; }
    
        // Init may be called multiple times so fMeta and fUI are reinitialized
        void init(const std::string& name,
                  const std::string& filename,
                  int inputs,
                  int outputs,
                  int sr_index,
                  const std::string& sha_key,
                  const std::string& dsp_code,
                  const std::string& version,
                  const std::string& compile_options,
                  const std::vector<std::string>& library_list,
                  const std::vector<std::string>& include_pathnames,
                  int size,
                  const PathTableType& path_table,
                  MemoryLayoutType memory_layout,
                  InstComplexity inst_comp,
                  bool extended = false)
        {
            fTab = 1;
            fExtended = extended;
            if (fExtended) {
                fUI << std::setprecision(std::numeric_limits<REAL>::max_digits10);
                fMeta << std::setprecision(std::numeric_limits<REAL>::max_digits10);
            }
        
            fIComp = inst_comp;
            
            // Start Meta generation
            fMeta.str("");
            tab(fTab, fMeta); fMeta << "\"meta\": [";
            fCloseMetaPar = ' ';
            
            // Start UI generation
            fUI.str("");
            tab(fTab, fUI); fUI << "\"ui\": [";
            fCloseUIPar = ' ';
            fTab += 1;
            
            fName = name;
            fFileName = filename;
            fInputs = inputs;
            fOutputs = outputs;
            fSRIndex = sr_index;
            fExpandedCode = dsp_code;
            fSHAKey = sha_key;
            fDSPSize = size;
            fPathTable = path_table;
            fVersion = version;
            fCompileOptions = compile_options;
            fLibraryList = library_list;
            fIncludePathnames = include_pathnames;
            fMemoryLayout = memory_layout;
        }
   
        // -- widget's layouts
    
        virtual void openGenericBox(const char* label, const char* name)
        {
            pushLabel(label);
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << name << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"items\": [";
            fCloseUIPar = ' ';
            fTab += 1;
        }

        virtual void openTabBox(const char* label)
        {
            openGenericBox(label, "tgroup");
        }
    
        virtual void openHorizontalBox(const char* label)
        {
            openGenericBox(label, "hgroup");
        }
    
        virtual void openVerticalBox(const char* label)
        {
            openGenericBox(label, "vgroup");
        }
    
        virtual void closeBox()
        {
            if (popLabel()) {
                // Shortnames can be computed when all fullnames are known
                computeShortNames();
            }
            fTab -= 1;
            tab(fTab, fUI); fUI << "]";
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }
    
        // -- active widgets
  
        virtual void addGenericButton(const char* label, const char* type, const char* varname)
        {
            std::string path = buildPath(label);
            fFullPaths.push_back(path);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << type << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            if (varname) { tab(fTab, fUI); fUI << "\"varname\": \"" << varname << "\","; }
        
            // Generate 'shortname' entry
            tab(fTab, fUI); fUI << "\"shortname\": \"";
        
            // Add fUI section
            fAllUI.push_back(fUI.str());
            fUI.str("");
        
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ((fMetaAux.size() > 0) ? "," : "");
            } else {
                tab(fTab, fUI); fUI << "\"address\": \"" << path << "\"" << ((fMetaAux.size() > 0) ? "," : "");
            }
            addMeta(fTab, false);
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        virtual void addButton(const char* label, REAL* zone)
        {
            addGenericButton(label, "button", nullptr);
        }
    
        void addButtonVarname(const char* label, const char* varname, REAL* zone)
        {
            addGenericButton(label, "button", varname);
        }
    
        virtual void addCheckButton(const char* label, REAL* zone)
        {
            addGenericButton(label, "checkbox", nullptr);
        }
    
        void addCheckButtonVarname(const char* label, const char* varname, REAL* zone)
        {
            addGenericButton(label, "checkbox", varname);
        }

        virtual void addGenericRange(const char* label, const char* type, const char* varname, REAL init, REAL min, REAL max, REAL step)
        {
            std::string path = buildPath(label);
            fFullPaths.push_back(path);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << type << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            if (varname) { tab(fTab, fUI); fUI << "\"varname\": \"" << varname << "\","; }
         
            // Generate 'shortname' entry
            tab(fTab, fUI); fUI << "\"shortname\": \"";
        
            // Add fUI section
            fAllUI.push_back(fUI.str());
            fUI.str("");
        
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ",";
            }
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"init\": " << init << ",";
            tab(fTab, fUI); fUI << "\"min\": " << min << ",";
            tab(fTab, fUI); fUI << "\"max\": " << max << ",";
            tab(fTab, fUI); fUI << "\"step\": " << step;
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }
    
        virtual void addVerticalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericRange(label, "vslider", nullptr, init, min, max, step);
        }
    
        void addVerticalSliderVarname(const char* label, const char* varname, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericRange(label, "vslider", varname, init, min, max, step);
        }
    
        virtual void addHorizontalSlider(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericRange(label, "hslider", nullptr, init, min, max, step);
        }
    
        void addHorizontalSliderVarname(const char* label, const char* varname, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericRange(label, "hslider", varname, init, min, max, step);
        }
    
        virtual void addNumEntry(const char* label, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericRange(label, "nentry", nullptr, init, min, max, step);
        }
    
        void addNumEntryVarname(const char* label, const char* varname, REAL* zone, REAL init, REAL min, REAL max, REAL step)
        {
            addGenericRange(label, "nentry", varname, init, min, max, step);
        }

        // -- passive widgets
    
        virtual void addGenericBargraph(const char* label, const char* type, const char* varname, REAL min, REAL max)
        {
            std::string path = buildPath(label);
            fFullPaths.push_back(path);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << type << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\",";
            if (varname) { tab(fTab, fUI); fUI << "\"varname\": \"" << varname << "\","; }
         
            // Generate 'shortname' entry
            tab(fTab, fUI); fUI << "\"shortname\": \"";
        
            // Add fUI section
            fAllUI.push_back(fUI.str());
            fUI.str("");
            
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\",";
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path) << ",";
            }
            addMeta(fTab);
            tab(fTab, fUI); fUI << "\"min\": " << min << ",";
            tab(fTab, fUI); fUI << "\"max\": " << max;
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }

        virtual void addHorizontalBargraph(const char* label, REAL* zone, REAL min, REAL max) 
        {
            addGenericBargraph(label, "hbargraph", nullptr, min, max);
        }
    
        void addHorizontalBargraphVarname(const char* label, const char* varname, REAL* zone, REAL min, REAL max)
        {
            addGenericBargraph(label, "hbargraph", varname, min, max);
        }
    
        virtual void addVerticalBargraph(const char* label, REAL* zone, REAL min, REAL max)
        {
            addGenericBargraph(label, "vbargraph", nullptr, min, max);
        }
    
        void addVerticalBargraphVarname(const char* label, const char* varname, REAL* zone, REAL min, REAL max)
        {
            addGenericBargraph(label, "vbargraph", varname, min, max);
        }
    
        virtual void addGenericSoundfile(const char* label, const char* varname, const char* url, Soundfile** zone)
        {
            std::string path = buildPath(label);
            fFullPaths.push_back(path);
            
            fUI << fCloseUIPar;
            tab(fTab, fUI); fUI << "{";
            fTab += 1;
            tab(fTab, fUI); fUI << "\"type\": \"" << "soundfile" << "\",";
            tab(fTab, fUI); fUI << "\"label\": \"" << label << "\"" << ",";
            if (varname) { tab(fTab, fUI); fUI << "\"varname\": \"" << varname << "\","; }
        
            // Generate 'shortname' entry
            tab(fTab, fUI); fUI << "\"shortname\": \"";
        
            // Add fUI section
            fAllUI.push_back(fUI.str());
            fUI.str("");
        
            tab(fTab, fUI); fUI << "\"url\": \"" << url << "\"" << ",";
            tab(fTab, fUI); fUI << "\"address\": \"" << path << "\"" << ((fPathTable.size() > 0) ? "," : "");
            if (fPathTable.size() > 0) {
                tab(fTab, fUI); fUI << "\"index\": " << getAddressIndex(path);
            }
            fTab -= 1;
            tab(fTab, fUI); fUI << "}";
            fCloseUIPar = ',';
        }
    
        virtual void addSoundfile(const char* label, const char* url, Soundfile** zone)
        {
            addGenericSoundfile(label, nullptr, url, zone);
        }
    
        void addSoundfileVarname(const char* label, const char* varname, const char* url, Soundfile** zone)
        {
            addGenericSoundfile(label, varname, url, zone);
        }

        // -- metadata declarations

        virtual void declare(REAL* zone, const char* key, const char* val)
        {
            fMetaAux.push_back(std::make_pair(key, val));
        }
    
        // Meta interface
        virtual void declare(const char* key, const char* value)
        {
            fMeta << fCloseMetaPar;
            // fName found in metadata
            if ((strcmp(key, "name") == 0) && (fName == "")) fName = value;
            // fFileName found in metadata
            if ((strcmp(key, "filename") == 0) && (fFileName == "")) fFileName = value;
            tab(fTab, fMeta); fMeta << "{ " << "\"" << key << "\"" << ": " << "\"" << value << "\" }";
            fCloseMetaPar = ',';
        }

        std::string JSON(bool flat = false)
        {
            if (fJSON.empty()) {
                fTab = 0;
                std::stringstream JSON;
                if (fExtended) {
                    JSON << std::setprecision(std::numeric_limits<REAL>::max_digits10);
                }
                JSON << "{";
                fTab += 1;
                tab(fTab, JSON); JSON << "\"name\": \"" << fName << "\",";
                tab(fTab, JSON); JSON << "\"filename\": \"" << fFileName << "\",";
                if (fVersion != "") { tab(fTab, JSON); JSON << "\"version\": \"" << fVersion << "\","; }
                if (fCompileOptions != "") { tab(fTab, JSON); JSON << "\"compile_options\": \"" <<  fCompileOptions << "\","; }
                if (fLibraryList.size() > 0) {
                    tab(fTab, JSON);
                    JSON << "\"library_list\": [";
                    for (size_t i = 0; i < fLibraryList.size(); i++) {
                        JSON << "\"" << fLibraryList[i] << "\"";
                        if (i < (fLibraryList.size() - 1)) JSON << ",";
                    }
                    JSON << "],";
                }
                if (fIncludePathnames.size() > 0) {
                    tab(fTab, JSON);
                    JSON << "\"include_pathnames\": [";
                    for (size_t i = 0; i < fIncludePathnames.size(); i++) {
                        JSON << "\"" << fIncludePathnames[i] << "\"";
                        if (i < (fIncludePathnames.size() - 1)) JSON << ",";
                    }
                    JSON << "],";
                }
                if (fDSPSize != -1) { tab(fTab, JSON); JSON << "\"size\": " << fDSPSize << ","; }
                if (fMemoryLayout.size() > 0) {
                    tab(fTab, JSON);
                    JSON << "\"memory_layout\": [";
                    for (size_t i = 0; i < fMemoryLayout.size(); i++) {
                        // DSP or field name, type, size, size-in-bytes, reads, writes
                        MemoryLayoutItem item = fMemoryLayout[i];
                        tab(fTab + 1, JSON);
                        JSON << "{ \"name\": \"" << item.name << "\", ";
                        JSON << "\"type\": \"" << item.type << "\", ";
                        JSON << "\"size\": " << item.size << ", ";
                        JSON << "\"size_bytes\": " << item.size_bytes << ", ";
                        JSON << "\"read\": " << item.read << ", ";
                        JSON << "\"write\": " << item.write << " }";
                        if (i < (fMemoryLayout.size() - 1)) JSON << ",";
                    }
                    tab(fTab, JSON);
                    JSON << "],";
                    
                    // Compute statistics
                    tab(fTab, JSON);
                    JSON << "\"compute_cost\": [{";
                    tab(fTab + 1, JSON);
                    JSON << "\"load\": " << fIComp.fLoad << ", ";
                    tab(fTab + 1, JSON);
                    JSON << "\"store\": " << fIComp.fStore << ", ";
                    tab(fTab + 1, JSON);
                    JSON << "\"declare\": " << fIComp.fDeclare << ", ";
                    tab(fTab + 1, JSON);
                    JSON << "\"number\": " << fIComp.fNumbers << ", ";
                    tab(fTab + 1, JSON);
                    JSON << "\"cast\": " << fIComp.fCast << ", ";
                    tab(fTab + 1, JSON);
                    JSON << "\"select\": " << fIComp.fSelect << ", ";
                    tab(fTab + 1, JSON);
                    JSON << "\"loop\": " << fIComp.fLoop << ", ";
                    tab(fTab + 1, JSON);
                    JSON << "\"binop\": [{ ";
                    JSON << "\"total\": " << fIComp.fBinop;
                    int size1 = (int)fIComp.fBinopSymbolTable.size();
                    if (size1 > 0) {
                        JSON << ", ";
                        for (const auto& it : fIComp.fBinopSymbolTable) {
                            JSON << "\"" << it.first << "\": " << it.second;
                            JSON << ((--size1 == 0) ? " }" : ", ");
                        }
                    } else {
                        JSON << " }";
                    }
                    JSON << "], ";
                    tab(fTab + 1, JSON);
                    JSON << "\"mathop\": [{ ";
                    JSON << "\"total\": " << fIComp.fMathop;
                    int size2 = (int)fIComp.fFunctionSymbolTable.size();
                    if (size2 > 0) {
                        JSON << ", ";
                        for (const auto& it : fIComp.fFunctionSymbolTable) {
                            JSON << "\"" << it.first << "\": " << it.second;
                            JSON << ((--size2 == 0) ? " }" : ", ");
                        }
                    } else {
                        JSON << " }";
                    }
                    JSON << "]";
                    tab(fTab, JSON);
                    JSON << "}],";
                }
                if (fSHAKey != "") { tab(fTab, JSON); JSON << "\"sha_key\": \"" << fSHAKey << "\","; }
                if (fExpandedCode != "") { tab(fTab, JSON); JSON << "\"code\": \"" << fExpandedCode << "\","; }
                tab(fTab, JSON); JSON << "\"inputs\": " << fInputs << ",";
                tab(fTab, JSON); JSON << "\"outputs\": " << fOutputs << ",";
                if (fSRIndex != -1) { tab(fTab, JSON); JSON << "\"sr_index\": " << fSRIndex << ","; }
                tab(fTab, fMeta); fMeta << "],";
              
                // Add last UI section
                fAllUI.push_back(fUI.str());
                // Finalize UI generation
                fUI.str("");
                // Add N-1 sections
                for (size_t i = 0; i < fAllUI.size()-1; i++) {
                    fUI << fAllUI[i] << fFull2Short[fFullPaths[i]] << "\",";
                }
                // And the last one
                fUI << fAllUI[fAllUI.size()-1];
                // Terminates the UI section
                tab(fTab, fUI); fUI << "]";
            
                fTab -= 1;
                if (fCloseMetaPar == ',') { // If "declare" has been called, fCloseMetaPar state is now ','
                    JSON << fMeta.str() << fUI.str();
                } else {
                    JSON << fUI.str();
                }
                
                tab(fTab, JSON); JSON << "}";
                
                // Keep result in fJSON
                fJSON = JSON.str();
            }
            return (flat) ? flatten(fJSON) : fJSON;
        }

        // Stream JSON to a caller-provided output to avoid extra copies.
        void writeJSON(std::ostream& out, bool flat = false)
        {
            const std::string& json = JSON(flat);
            out << json;
        }
    
};

// Externally available class using FAUSTFLOAT

struct FAUST_API JSONUI : public JSONUIReal<FAUSTFLOAT>, public UI {
    
    JSONUI(const std::string& name,
           const std::string& filename,
           int inputs,
           int outputs,
           int sr_index,
           const std::string& sha_key,
           const std::string& dsp_code,
           const std::string& version,
           const std::string& compile_options,
           const std::vector<std::string>& library_list,
           const std::vector<std::string>& include_pathnames,
           int size,
           const PathTableType& path_table,
           MemoryLayoutType memory_layout,
           InstComplexity inst_comp):
    JSONUIReal<FAUSTFLOAT>(name, filename,
                          inputs, outputs,
                          sr_index,
                          sha_key, dsp_code,
                          version, compile_options,
                          library_list, include_pathnames,
                          size, path_table,
                          memory_layout, inst_comp)
    {}
    
    JSONUI(const std::string& name, const std::string& filename, int inputs, int outputs):
    JSONUIReal<FAUSTFLOAT>(name, filename, inputs, outputs)
    {}
    
    JSONUI(int inputs, int outputs):JSONUIReal<FAUSTFLOAT>(inputs, outputs)
    {}
    
    JSONUI():JSONUIReal<FAUSTFLOAT>()
    {}

    virtual void openTabBox(const char* label)
    {
        JSONUIReal<FAUSTFLOAT>::openTabBox(label);
    }
    virtual void openHorizontalBox(const char* label)
    {
        JSONUIReal<FAUSTFLOAT>::openHorizontalBox(label);
    }
    virtual void openVerticalBox(const char* label)
    {
        JSONUIReal<FAUSTFLOAT>::openVerticalBox(label);
    }
    virtual void closeBox()
    {
        JSONUIReal<FAUSTFLOAT>::closeBox();
    }
    
    // -- active widgets
    
    virtual void addButton(const char* label, FAUSTFLOAT* zone)
    {
        JSONUIReal<FAUSTFLOAT>::addButton(label, zone);
    }
    void addButtonVarname(const char* label, const char* varname, FAUSTFLOAT* zone)
    {
        JSONUIReal<FAUSTFLOAT>::addButtonVarname(label, varname, zone);
    }
    virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
    {
        JSONUIReal<FAUSTFLOAT>::addCheckButton(label, zone);
    }
    void addCheckButtonVarname(const char* label, const char* varname, FAUSTFLOAT* zone)
    {
        JSONUIReal<FAUSTFLOAT>::addCheckButtonVarname(label, varname, zone);
    }
    virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addVerticalSlider(label, zone, init, min, max, step);
    }
    void addVerticalSliderVarname(const char* label, const char* varname, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addVerticalSliderVarname(label, varname, zone, init, min, max, step);
    }
    virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addHorizontalSlider(label, zone, init, min, max, step);
    }
    void addHorizontalSliderVarname(const char* label, const char* varname, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addHorizontalSliderVarname(label, varname, zone, init, min, max, step);
    }
    virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addNumEntry(label, zone, init, min, max, step);
    }
    void addNumEntryVarname(const char* label, const char* varname, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
    {
        JSONUIReal<FAUSTFLOAT>::addNumEntryVarname(label, varname, zone, init, min, max, step);
    }
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIReal<FAUSTFLOAT>::addHorizontalBargraph(label, zone, min, max);
    }
    void addHorizontalBargraphVarname(const char* label, const char* varname, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIReal<FAUSTFLOAT>::addHorizontalBargraphVarname(label, varname, zone, min, max);
    }
    virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIReal<FAUSTFLOAT>::addVerticalBargraph(label, zone, min, max);
    }
    void addVerticalBargraphVarname(const char* label, const char* varname, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
    {
        JSONUIReal<FAUSTFLOAT>::addVerticalBargraphVarname(label, varname, zone, min, max);
    }
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* url, Soundfile** sf_zone)
    {
        JSONUIReal<FAUSTFLOAT>::addSoundfile(label, url, sf_zone);
    }
    void addSoundfileVarname(const char* label, const char* varname, const char* url, Soundfile** sf_zone)
    {
        JSONUIReal<FAUSTFLOAT>::addSoundfileVarname(label, varname, url, sf_zone);
    }
    
    // -- metadata declarations
    
    virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
    {
        JSONUIReal<FAUSTFLOAT>::declare(zone, key, val);
    }

    virtual void declare(const char* key, const char* val)
    {
        JSONUIReal<FAUSTFLOAT>::declare(key, val);
    }

    virtual ~JSONUI() {}
    
};

#endif // FAUST_JSONUI_H
/**************************  END  JSONUI.h **************************/
/************************** BEGIN midi.h *******************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
************************************************************************/

#ifndef __midi__
#define __midi__

#include <vector>
#include <string>
#include <string.h>
#include <algorithm>
#include <cstdint>
#include <assert.h>


class FAUST_API MapUI;

/**
 * A timestamped short MIDI message used with SOUL.
 */

// Force contiguous memory layout
#pragma pack (push, 1)
struct MIDIMessage
{
    uint32_t frameIndex;
    uint8_t byte0, byte1, byte2;
};
#pragma pack (pop)

/**
 * For timestamped MIDI messages (in usec).
 */
struct DatedMessage {
    
    double fDate;
    unsigned char fBuffer[3];
    size_t fSize;
    
    DatedMessage(double date, unsigned char* buffer, size_t size)
    :fDate(date), fSize(size)
    {
        assert(size <= 3);
        memcpy(fBuffer, buffer, size);
    }
    
    DatedMessage():fDate(0.0), fSize(0)
    {}
    
};

/**
 * MIDI processor definition.
 *
 * MIDI input or output handling classes will implement this interface,
 * so the same method names (keyOn, keyOff, ctrlChange...) will be used either
 * when decoding MIDI input or encoding MIDI output events.
 * MIDI channel is numbered in [0..15] in this layer.
 */
class midi {

    public:

        midi() {}
        virtual ~midi() {}

        // Additional timestamped API for MIDI input
        virtual MapUI* keyOn(double, int channel, int pitch, int velocity)
        {
            return keyOn(channel, pitch, velocity);
        }
        
        virtual void keyOff(double, int channel, int pitch, int velocity = 0)
        {
            keyOff(channel, pitch, velocity);
        }
    
        virtual void keyPress(double, int channel, int pitch, int press)
        {
            keyPress(channel, pitch, press);
        }
        
        virtual void chanPress(double date, int channel, int press)
        {
            chanPress(channel, press);
        }
    
        virtual void pitchWheel(double, int channel, int wheel)
        {
            pitchWheel(channel, wheel);
        }
           
        virtual void ctrlChange(double, int channel, int ctrl, int value)
        {
            ctrlChange(channel, ctrl, value);
        }
    
        virtual void ctrlChange14bits(double, int channel, int ctrl, int value)
        {
            ctrlChange14bits(channel, ctrl, value);
        }
    
        virtual void rpn(double, int channel, int ctrl, int value)
        {
            rpn(channel, ctrl, value);
        }

        virtual void progChange(double, int channel, int pgm)
        {
            progChange(channel, pgm);
        }
    
        virtual void sysEx(double, std::vector<unsigned char>& message)
        {
            sysEx(message);
        }

        // MIDI sync
        virtual void startSync(double date)  {}
        virtual void stopSync(double date)   {}
        virtual void clock(double date)  {}

        // Standard MIDI API
        virtual MapUI* keyOn(int channel, int pitch, int velocity)      { return nullptr; }
        virtual void keyOff(int channel, int pitch, int velocity)       {}
        virtual void keyPress(int channel, int pitch, int press)        {}
        virtual void chanPress(int channel, int press)                  {}
        virtual void ctrlChange(int channel, int ctrl, int value)       {}
        virtual void ctrlChange14bits(int channel, int ctrl, int value) {}
        virtual void rpn(int channel, int ctrl, int value)              {}
        virtual void pitchWheel(int channel, int wheel)                 {}
        virtual void progChange(int channel, int pgm)                   {}
        virtual void sysEx(std::vector<unsigned char>& message)         {}

        enum MidiStatus {
            // channel voice messages
            MIDI_NOTE_OFF = 0x80,
            MIDI_NOTE_ON = 0x90,
            MIDI_CONTROL_CHANGE = 0xB0,
            MIDI_PROGRAM_CHANGE = 0xC0,
            MIDI_PITCH_BEND = 0xE0,
            MIDI_AFTERTOUCH = 0xD0,         // aka channel pressure
            MIDI_POLY_AFTERTOUCH = 0xA0,    // aka key pressure
            MIDI_CLOCK = 0xF8,
            MIDI_START = 0xFA,
            MIDI_CONT = 0xFB,
            MIDI_STOP = 0xFC,
            MIDI_SYSEX_START = 0xF0,
            MIDI_SYSEX_STOP = 0xF7
        };

        enum MidiCtrl {
            ALL_NOTES_OFF = 123,
            ALL_SOUND_OFF = 120
        };
    
        enum MidiNPN {
            PITCH_BEND_RANGE = 0
        };

};

/**
 * A class to decode NRPN and RPN messages, adapted from JUCE forum message:
 * https://forum.juce.com/t/14bit-midi-controller-support/11517
 */
class MidiNRPN {
    
    private:
    
        bool ctrlnew;
        int ctrlnum;
        int ctrlval;
        
        int nrpn_lsb, nrpn_msb;
        int data_lsb, data_msb;
        
        enum
        {
            midi_nrpn_lsb = 98,
            midi_nrpn_msb = 99,
            midi_rpn_lsb  = 100,
            midi_rpn_msb  = 101,
            midi_data_lsb = 38,
            midi_data_msb = 6
        };
    
    public:
        
        MidiNRPN(): ctrlnew(false), nrpn_lsb(-1), nrpn_msb(-1), data_lsb(-1), data_msb(-1)
        {}
        
        // return true if the message has been filtered
        bool process(int data1, int data2)
        {
            switch (data1)
            {
                case midi_nrpn_lsb: nrpn_lsb = data2; return true;
                case midi_nrpn_msb: nrpn_msb = data2; return true;
                case midi_rpn_lsb: {
                    if (data2 == 127) {
                        nrpn_lsb = data_lsb = -1;
                    } else {
                        nrpn_lsb = 0;
                        data_lsb = -1;
                    }
                    return true;
                }
                case midi_rpn_msb: {
                    if (data2 == 127) {
                        nrpn_msb = data_msb = -1;
                    } else {
                        nrpn_msb = 0;
                        data_msb = -1;
                    }
                    return true;
                }
                case midi_data_lsb:
                case midi_data_msb:
                {
                    if (data1 == midi_data_msb) {
                        if (nrpn_msb < 0) {
                            return false;
                        }
                        data_msb = data2;
                    } else { // midi_data_lsb
                        if (nrpn_lsb < 0) {
                            return false;
                        }
                        data_lsb = data2;
                    }
                    if (data_lsb >= 0 && data_msb >= 0) {
                        ctrlnum = (nrpn_msb << 7) | nrpn_lsb;
                        ctrlval = (data_msb << 7) | data_lsb;
                        data_lsb = data_msb = -1;
                        nrpn_msb = nrpn_lsb = -1;
                        ctrlnew = true;
                    }
                    return true;
                }
                default: return false;
            };
        }
        
        bool hasNewNRPN() { bool res = ctrlnew; ctrlnew = false; return res; }
        
        // results in [0, 16383]
        int getCtrl() const { return ctrlnum; }
        int getVal() const { return ctrlval; }
    
};

/**
 * A pure interface for MIDI handlers that can send/receive MIDI messages to/from 'midi' objects.
 */
struct midi_interface {
    virtual void addMidiIn(midi* midi_dsp)      = 0;
    virtual void removeMidiIn(midi* midi_dsp)   = 0;
    virtual ~midi_interface() {}
};

/****************************************************
 * Base class for MIDI input handling.
 *
 * Shared common code used for input handling:
 * - decoding Real-Time messages: handleSync
 * - decoding one data byte messages: handleData1
 * - decoding two data byte messages: handleData2
 * - getting ready messages in polling mode
 ****************************************************/
class midi_handler : public midi, public midi_interface {

    protected:

        std::vector<midi*> fMidiInputs;
        std::string fName;
        MidiNRPN fNRPN;
    
        int range(int min, int max, int val) { return (val < min) ? min : ((val >= max) ? max : val); }
  
    public:

        midi_handler(const std::string& name = "MIDIHandler"):midi_interface(), fName(name) {}
        virtual ~midi_handler() {}

        void addMidiIn(midi* midi_dsp) { if (midi_dsp) fMidiInputs.push_back(midi_dsp); }
        void removeMidiIn(midi* midi_dsp)
        {
            std::vector<midi*>::iterator it = std::find(fMidiInputs.begin(), fMidiInputs.end(), midi_dsp);
            if (it != fMidiInputs.end()) {
                fMidiInputs.erase(it);
            }
        }

        // Those 2 methods have to be implemented by subclasses
        virtual bool startMidi() { return true; }
        virtual void stopMidi() {}
    
        void setName(const std::string& name) { fName = name; }
        std::string getName() { return fName; }
    
        // To be used in polling mode
        virtual int recvMessages(std::vector<MIDIMessage>* message) { return 0; }
        virtual void sendMessages(std::vector<MIDIMessage>* message, int count) {}
    
        // MIDI Real-Time
        void handleClock(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->clock(time);
            }
        }
        
        void handleStart(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->startSync(time);
            }
        }
        
        void handleStop(double time)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->stopSync(time);
            }
        }
    
        void handleSync(double time, int type)
        {
            if (type == MIDI_CLOCK) {
                handleClock(time);
            // We can consider start and continue as identical messages
            } else if ((type == MIDI_START) || (type == MIDI_CONT)) {
                handleStart(time);
            } else if (type == MIDI_STOP) {
                handleStop(time);
            }
        }
    
        // MIDI 1 data
        void handleProgChange(double time, int channel, int data1)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->progChange(time, channel, data1);
            }
        }
    
        void handleAfterTouch(double time, int channel, int data1)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->chanPress(time, channel, data1);
            }
        }

        void handleData1(double time, int type, int channel, int data1)
        {
            if (type == MIDI_PROGRAM_CHANGE) {
                handleProgChange(time, channel, data1);
            } else if (type == MIDI_AFTERTOUCH) {
                handleAfterTouch(time, channel, data1);
            }
        }
    
        // MIDI 2 datas
        void handleKeyOff(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->keyOff(time, channel, data1, data2);
            }
        }
        
        void handleKeyOn(double time, int channel, int data1, int data2)
        {
            if (data2 == 0) {
                handleKeyOff(time, channel, data1, data2);
            } else {
                for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                    fMidiInputs[i]->keyOn(time, channel, data1, data2);
                }
            }
        }
    
        void handleCtrlChange(double time, int channel, int data1, int data2)
        {
            // Special processing for NRPN and RPN
            if (fNRPN.process(data1, data2)) {
                if (fNRPN.hasNewNRPN()) {
                    for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                        fMidiInputs[i]->rpn(time, channel, fNRPN.getCtrl(), fNRPN.getVal());
                    }
                }
            } else {
                for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                    fMidiInputs[i]->ctrlChange(time, channel, data1, data2);
                }
            }
        }
        
        void handlePitchWheel(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->pitchWheel(time, channel, (data2 << 7) + data1);
            }
        }
    
        void handlePitchWheel(double time, int channel, int bend)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->pitchWheel(time, channel, bend);
            }
        }
        
        void handlePolyAfterTouch(double time, int channel, int data1, int data2)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->keyPress(time, channel, data1, data2);
            }
        }
  
        void handleData2(double time, int type, int channel, int data1, int data2)
        {
            if (type == MIDI_NOTE_OFF) {
                handleKeyOff(time, channel,  data1, data2);
            } else if (type == MIDI_NOTE_ON) {
                handleKeyOn(time, channel, data1, data2);
            } else if (type == MIDI_CONTROL_CHANGE) {
                handleCtrlChange(time, channel, data1, data2);
            } else if (type == MIDI_PITCH_BEND) {
                handlePitchWheel(time, channel, data1, data2);
            } else if (type == MIDI_POLY_AFTERTOUCH) {
                handlePolyAfterTouch(time, channel, data1, data2);
            }
        }
    
        // SysEx
        void handleSysex(double time, std::vector<unsigned char>& message)
        {
            for (unsigned int i = 0; i < fMidiInputs.size(); i++) {
                fMidiInputs[i]->sysEx(time, message);
            }
        }
    
        void handleMessage(double time, int type, std::vector<unsigned char>& message)
        {
            if (type == MIDI_SYSEX_START) {
                handleSysex(time, message);
            }
        }
  
};

#define ucast(v) static_cast<unsigned char>(v)

#endif // __midi__
/**************************  END  midi.h **************************/

#ifdef _MSC_VER
#define gsscanf sscanf_s
#else
#define gsscanf sscanf
#endif

/**
 * Helper code for MIDI meta and polyphonic 'nvoices' parsing.
 */
struct MidiMeta : public Meta {
    
    std::map<std::string, std::string> fData;
    
    void declare(const char* key, const char* value) override
    {
        fData[key] = value;
    }
    
    const std::string get(const char* key, const char* def) const noexcept
    {
        auto it = fData.find(key);
        return (it != fData.end()) ? it->second : def;
    }
    
    static void analyse(dsp* mono_dsp, bool& midi, bool& midi_sync, int& nvoices)
    {
        JSONUI jsonui;
        mono_dsp->buildUserInterface(&jsonui);
        std::string json = jsonui.JSON();
        midi_sync = ((json.find("midi") != std::string::npos) &&
                     ((json.find("start") != std::string::npos) ||
                      (json.find("stop") != std::string::npos) ||
                      (json.find("clock") != std::string::npos) ||
                      (json.find("timestamp") != std::string::npos)));
    
    #if defined(NVOICES) && NVOICES!=NUM_VOICES
        // Compile-time override
        nvoices = NVOICES;
    #else
        MidiMeta meta;
        mono_dsp->metadata(&meta);
        bool found_voices = false;
        bool found_midi = false;
        // If "options" metadata is used
        std::string options = meta.get("options", "");
        if (options != "") {
            std::map<std::string, std::string> metadata;
            std::string res;
            MetaDataUI::extractMetadata(options, res, metadata);
            if (metadata.find("nvoices") != metadata.end()) {
                nvoices = std::atoi(metadata["nvoices"].c_str());
                found_voices = true;
            }
            if (metadata.find("midi") != metadata.end()) {
                midi = (metadata["midi"] == "on" || metadata["midi"] == "1");
                found_midi = true;
            }
        }
        // Otherwise test for "nvoices" metadata
        if (!found_voices) {
            std::string numVoices = meta.get("nvoices", "0");
            nvoices = std::atoi(numVoices.c_str());
        }
        nvoices = std::max<int>(0, nvoices);
        // Otherwise test for "midi" metadata
        if (!found_midi) {
            std::string midiState = meta.get("midi", "off");
            midi = (midiState == "on" || midiState == "1");
        }
    #endif
    }
    
    static bool checkPolyphony(dsp* mono_dsp)
    {
        MapUI map_ui;
        mono_dsp->buildUserInterface(&map_ui);
        bool has_freq = false;
        bool has_gate = false;
        bool has_gain = false;
        for (int i = 0; i < map_ui.getParamsCount(); i++) {
            std::string path = map_ui.getParamAddress(i);
            has_freq |= MapUI::endsWith(path, "/freq");
            has_freq |= MapUI::endsWith(path, "/key");
            has_gate |= MapUI::endsWith(path, "/gate");
            has_gain |= MapUI::endsWith(path, "/gain");
            has_gain |= MapUI::endsWith(path, "/vel");
            has_gain |= MapUI::endsWith(path, "/velocity");
        }
        return (has_freq && has_gate && has_gain);
    }
    
};

/**
 * uiMidi : Faust User Interface
 * This class decodes MIDI meta data and maps incoming MIDI messages to them.
 * Currently ctrlChange, keyOn/keyOff, keyPress, progChange, chanPress, pitchWheel/pitchBend
 * start/stop/clock meta data is handled.
 * MIDI channel is numbered in [1..16] in this layer.
 * Channel 0 means "all channels" when receiving or sending.
 */
class uiMidi {
    
    friend class MidiUI;
    
    protected:
        
        midi* fMidiOut;
        bool fInputCtrl;
        int fChan;
    
        bool inRange(FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT v) { return (min <= v && v <= max); }
    
    public:
        
        uiMidi(midi* midi_out, bool input, int chan = 0):fMidiOut(midi_out), fInputCtrl(input), fChan(chan)
        {}
        virtual ~uiMidi()
        {}

};

/**
 * Base class for MIDI aware UI items.
 */
class uiMidiItem : public uiMidi, public uiItem {
    
    public:
        
        uiMidiItem(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = 0)
            :uiMidi(midi_out, input, chan), uiItem(ui, zone)
        {}
        virtual ~uiMidiItem()
        {}
    
        virtual void reflectZone() {}
    
};

/**
 * Base class for MIDI aware UI items with timestamp support.
 */
class uiMidiTimedItem : public uiMidi, public uiTimedItem {
    
    public:
        
        uiMidiTimedItem(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true, int chan = 0)
            :uiMidi(midi_out, input, chan), uiTimedItem(ui, zone)
        {}
        virtual ~uiMidiTimedItem()
        {}
    
        virtual void reflectZone() {}
    
};

/**
 * MIDI sync.
 */
class uiMidiStart : public uiMidiTimedItem
{
  
    public:
    
        uiMidiStart(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input)
        {}
        virtual ~uiMidiStart()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(0)) {
                fMidiOut->startSync(0);
            }
        }
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(v));
            }
        }
        
};

class uiMidiStop : public uiMidiTimedItem {
  
    public:
    
        uiMidiStop(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input)
        {}
        virtual ~uiMidiStop()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (v != FAUSTFLOAT(1)) {
                fMidiOut->stopSync(0);
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(v));
            }
        }
};

class uiMidiClock : public uiMidiTimedItem {

    private:
        
        bool fState;
  
    public:
    
        uiMidiClock(midi* midi_out, GUI* ui, FAUSTFLOAT* zone, bool input = true)
            :uiMidiTimedItem(midi_out, ui, zone, input), fState(false)
        {}
        virtual ~uiMidiClock()
        {}
    
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            fMidiOut->clock(0);
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                fState = !fState;
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fState));
            }
        }

};

/**
 * Standard MIDI events.
 */

/**
 * uiMidiProgChange uses the [min...max] range.
 */
class uiMidiProgChange : public uiMidiTimedItem {
    
    public:
    
        FAUSTFLOAT fMin, fMax;
    
        uiMidiProgChange(midi* midi_out, GUI* ui, FAUSTFLOAT* zone,
                         FAUSTFLOAT min, FAUSTFLOAT max,
                         bool input = true, int chan = 0)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), fMin(min), fMax(max)
        {}
        virtual ~uiMidiProgChange()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            if (inRange(fMin, fMax, v)) {
                if (fChan == 0) {
                    // Send on [0..15] channels on the MIDI layer
                    for (int chan = 0; chan < 16; chan++) {
                        fMidiOut->progChange(chan, v);
                    }
                } else {
                    fMidiOut->progChange(fChan - 1, v);
                }
            }
        }
    
        void modifyZone(FAUSTFLOAT v)
        {
            if (fInputCtrl && inRange(fMin, fMax, v)) {
                uiItem::modifyZone(v);
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl && inRange(fMin, fMax, v)) {
                uiMidiTimedItem::modifyZone(date, v);
            }
        }
        
};

/**
 * uiMidiChanPress.
 */
class uiMidiChanPress : public uiMidiTimedItem, public uiConverter {
    
    public:
    
        uiMidiChanPress(midi* midi_out, GUI* ui,
                        FAUSTFLOAT* zone,
                        FAUSTFLOAT min, FAUSTFLOAT max,
                        bool input = true,
                        MetaDataUI::Scale scale = MetaDataUI::kLin,
                        int chan = 0)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max)
        {}
        virtual ~uiMidiChanPress()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            int conv = std::round(fConverter->faust2ui(v));
            if (fChan == 0) {
                // Send on [0..15] channels on the MIDI layer
                for (int chan = 0; chan < 16; chan++) {
                    fMidiOut->chanPress(chan, conv);
                }
            } else {
                fMidiOut->chanPress(fChan - 1, conv);
            }
        }
    
        void modifyZone(FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
        
};

/**
 * uiMidiCtrlChange does scale (kLin/kLog/kExp) mapping.
 */
class uiMidiCtrlChange : public uiMidiTimedItem, public uiConverter {
    
    private:
    
        int fCtrl;
 
    public:

        uiMidiCtrlChange(midi* midi_out, int ctrl, GUI* ui,
                     FAUSTFLOAT* zone,
                     FAUSTFLOAT min, FAUSTFLOAT max,
                     bool input = true,
                     MetaDataUI::Scale scale = MetaDataUI::kLin,
                     int chan = 0)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fCtrl(ctrl)
        {}
        virtual ~uiMidiCtrlChange()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            int conv = std::round(fConverter->faust2ui(v));
            if (fChan == 0) {
                // Send on [0..15] channels on the MIDI layer
                for (int chan = 0; chan < 16; chan++) {
                    fMidiOut->ctrlChange(chan, fCtrl, conv);
                }
            } else {
                fMidiOut->ctrlChange(fChan - 1, fCtrl, conv);
            }
        }
        
        void modifyZone(FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
};

// Use a two segments linear converter
class uiMidiPitchWheel : public uiMidiTimedItem {

    private:
    
        LinearValueConverter2 fConverter;
    
    public:
    
        uiMidiPitchWheel(midi* midi_out, GUI* ui, FAUSTFLOAT* zone,
                         FAUSTFLOAT min, FAUSTFLOAT max,
                         bool input = true, int chan = 0)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan)
        {
            if (min <= 0 && max >= 0) {
                fConverter = LinearValueConverter2(0., 8191., 16383., double(min), 0., double(max));
            } else {
                // Degenerated case...
                fConverter = LinearValueConverter2(0., 8191., 16383., double(min),double(min + (max - min)/FAUSTFLOAT(2)), double(max));
            }
        }
    
        virtual ~uiMidiPitchWheel()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            int conv = std::round(fConverter.faust2ui(v));
            if (fChan == 0) {
                // Send on [0..15] channels on the MIDI layer
                for (int chan = 0; chan < 16; chan++) {
                    fMidiOut->pitchWheel(chan, conv);
                }
            } else {
                fMidiOut->pitchWheel(fChan - 1, conv);
            }
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(FAUSTFLOAT(fConverter.ui2faust(v)));
            }
        }
    
        void setRange(int val)
        {
            double semi = (val / 128) + ((val % 128) / 100.);
            fConverter.setMappingValues(0., 8191., 16383., -semi, 0., semi);
        }
 
};

/**
 * uiMidiKeyOn does scale (kLin/kLog/kExp) mapping for velocity.
 */
class uiMidiKeyOn : public uiMidiTimedItem, public uiConverter {

    private:
        
        int fKeyOn;
  
    public:
    
        uiMidiKeyOn(midi* midi_out, int key, GUI* ui,
                    FAUSTFLOAT* zone,
                    FAUSTFLOAT min, FAUSTFLOAT max,
                    bool input = true,
                    MetaDataUI::Scale scale = MetaDataUI::kLin,
                    int chan = 0)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fKeyOn(key)
        {}
        virtual ~uiMidiKeyOn()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            int conv = std::round(fConverter->faust2ui(v));
            if (fChan == 0) {
                // Send on [0..15] channels on the MIDI layer
                for (int chan = 0; chan < 16; chan++) {
                    fMidiOut->keyOn(chan, fKeyOn, conv);
                }
            } else {
                fMidiOut->keyOn(fChan - 1, fKeyOn, conv);
            }
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
};

/**
 * uiMidiKeyOff does scale (kLin/kLog/kExp) mapping for velocity.
 */
class uiMidiKeyOff : public uiMidiTimedItem, public uiConverter {

    private:
        
        int fKeyOff;
  
    public:
    
        uiMidiKeyOff(midi* midi_out, int key, GUI* ui,
                     FAUSTFLOAT* zone,
                     FAUSTFLOAT min, FAUSTFLOAT max,
                     bool input = true,
                     MetaDataUI::Scale scale = MetaDataUI::kLin,
                     int chan = 0)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fKeyOff(key)
        {}
        virtual ~uiMidiKeyOff()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            int conv = std::round(fConverter->faust2ui(v));
            if (fChan == 0) {
                // Send on [0..15] channels on the MIDI layer
                for (int chan = 0; chan < 16; chan++) {
                    fMidiOut->keyOff(chan, fKeyOff, conv);
                }
            } else {
                fMidiOut->keyOff(fChan - 1, fKeyOff, conv);
            }
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
};

/**
 * uiMidiKeyPress does scale (kLin/kLog/kExp) mapping for velocity.
 */
class uiMidiKeyPress : public uiMidiTimedItem, public uiConverter {

    private:
    
        int fKey;
  
    public:
    
        uiMidiKeyPress(midi* midi_out, int key, GUI* ui,
                       FAUSTFLOAT* zone,
                       FAUSTFLOAT min, FAUSTFLOAT max,
                       bool input = true,
                       MetaDataUI::Scale scale = MetaDataUI::kLin,
                       int chan = 0)
            :uiMidiTimedItem(midi_out, ui, zone, input, chan), uiConverter(scale, 0., 127., min, max), fKey(key)
        {}
        virtual ~uiMidiKeyPress()
        {}
        
        virtual void reflectZone()
        {
            FAUSTFLOAT v = *fZone;
            fCache = v;
            int conv = std::round(fConverter->faust2ui(v));
            if (fChan == 0) {
                // Send on [0..15] channels on the MIDI layer
                for (int chan = 0; chan < 16; chan++) {
                    fMidiOut->keyPress(chan, fKey, conv);
                }
            } else {
                fMidiOut->keyPress(fChan - 1, fKey, conv);
            }
        }
        
        void modifyZone(FAUSTFLOAT v)
        { 
            if (fInputCtrl) {
                uiItem::modifyZone(FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
        void modifyZone(double date, FAUSTFLOAT v)
        {
            if (fInputCtrl) {
                uiMidiTimedItem::modifyZone(date, FAUSTFLOAT(fConverter->ui2faust(v)));
            }
        }
    
};

/******************************************************************************************
 * MidiUI : Faust User Interface
 * This class decodes MIDI metadata and maps incoming MIDI messages to them.
 * Currently ctrlChange, keyOn/keyOff, keyPress, progChange, chanPress, pitchWheel/pitchBend
 * start/stop/clock meta data are handled.
 *
 * Maps associating MIDI event ID (like each ctrl number) with all MIDI aware UI items
 * are defined and progressively filled when decoding MIDI related metadata.
 * MIDI aware UI items are used in both directions:
 *  - modifying their internal state when receving MIDI input events
 *  - sending their internal state as MIDI output events
 *******************************************************************************************/

class MidiUI : public GUI, public midi, public midi_interface, public MetaDataUI {

    // Add uiItem subclasses objects are deallocated by the inherited GUI class
    typedef std::map <int, std::vector<uiMidiCtrlChange*> > TCtrlChangeTable;
    typedef std::vector<uiMidiProgChange*>                  TProgChangeTable;
    typedef std::vector<uiMidiChanPress*>                   TChanPressTable;
    typedef std::map <int, std::vector<uiMidiKeyOn*> >      TKeyOnTable;
    typedef std::map <int, std::vector<uiMidiKeyOff*> >     TKeyOffTable;
    typedef std::map <int, std::vector<uiMidiKeyPress*> >   TKeyPressTable;
    typedef std::vector<uiMidiPitchWheel*>                  TPitchWheelTable;
    
    protected:
    
        TCtrlChangeTable fCtrlChangeTable;
        TProgChangeTable fProgChangeTable;
        TChanPressTable  fChanPressTable;
        TKeyOnTable      fKeyOnTable;
        TKeyOffTable     fKeyOffTable;
        TKeyOnTable      fKeyTable;
        TKeyPressTable   fKeyPressTable;
        TPitchWheelTable fPitchWheelTable;
        
        std::vector<uiMidiStart*> fStartTable;
        std::vector<uiMidiStop*>  fStopTable;
        std::vector<uiMidiClock*> fClockTable;
        
        std::vector<std::pair <std::string, std::string> > fMetaAux;
        
        midi_handler* fMidiHandler;
        bool fTimeStamp;
    
        void addGenericZone(FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max, bool input = true)
        {
            if (fMetaAux.size() > 0) {
                for (size_t i = 0; i < fMetaAux.size(); i++) {
                    unsigned num;
                    unsigned chan;
                    if (fMetaAux[i].first == "midi") {
                        if (gsscanf(fMetaAux[i].second.c_str(), "ctrl %u %u", &num, &chan) == 2) {
                            fCtrlChangeTable[num].push_back(new uiMidiCtrlChange(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "ctrl %u", &num) == 1) {
                            fCtrlChangeTable[num].push_back(new uiMidiCtrlChange(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyon %u %u", &num, &chan) == 2) {
                            fKeyOnTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyon %u", &num) == 1) {
                            fKeyOnTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyoff %u %u", &num, &chan) == 2) {
                            fKeyOffTable[num].push_back(new uiMidiKeyOff(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keyoff %u", &num) == 1) {
                            fKeyOffTable[num].push_back(new uiMidiKeyOff(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "key %u %u", &num, &chan) == 2) {
                            fKeyTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "key %u", &num) == 1) {
                            fKeyTable[num].push_back(new uiMidiKeyOn(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keypress %u %u", &num, &chan) == 2) {
                            fKeyPressTable[num].push_back(new uiMidiKeyPress(fMidiHandler, num, this, zone, min, max, input, getScale(zone), chan));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "keypress %u", &num) == 1) {
                            fKeyPressTable[num].push_back(new uiMidiKeyPress(fMidiHandler, num, this, zone, min, max, input, getScale(zone)));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "pgm %u", &chan) == 1) {
                            fProgChangeTable.push_back(new uiMidiProgChange(fMidiHandler, this, zone, min, max, input, chan));
                        } else if (strcmp(fMetaAux[i].second.c_str(), "pgm") == 0) {
                            fProgChangeTable.push_back(new uiMidiProgChange(fMidiHandler, this, zone, min, max, input));
                        } else if (gsscanf(fMetaAux[i].second.c_str(), "chanpress %u", &chan) == 1) {
                            fChanPressTable.push_back(new uiMidiChanPress(fMidiHandler, this, zone, min, max, input, getScale(zone), chan));
                        } else if ((fMetaAux[i].second == "chanpress")) {
                            fChanPressTable.push_back(new uiMidiChanPress(fMidiHandler, this, zone, min, max, input, getScale(zone)));
                        } else if ((gsscanf(fMetaAux[i].second.c_str(), "pitchwheel %u", &chan) == 1) || (gsscanf(fMetaAux[i].second.c_str(), "pitchbend %u", &chan) == 1)) {
                            fPitchWheelTable.push_back(new uiMidiPitchWheel(fMidiHandler, this, zone, min, max, input, chan));
                        } else if ((fMetaAux[i].second == "pitchwheel") || (fMetaAux[i].second == "pitchbend")) {
                            fPitchWheelTable.push_back(new uiMidiPitchWheel(fMidiHandler, this, zone, min, max, input));
                        // MIDI sync
                        } else if (fMetaAux[i].second == "start") {
                            fStartTable.push_back(new uiMidiStart(fMidiHandler, this, zone, input));
                        } else if (fMetaAux[i].second == "stop") {
                            fStopTable.push_back(new uiMidiStop(fMidiHandler, this, zone, input));
                        } else if (fMetaAux[i].second == "clock") {
                            fClockTable.push_back(new uiMidiClock(fMidiHandler, this, zone, input));
                        // Explicit metadata to activate 'timestamp' mode
                        } else if (fMetaAux[i].second == "timestamp") {
                            fTimeStamp = true;
                        }
                    }
                }
            }
            fMetaAux.clear();
        }
    
        template <typename TABLE>
        void updateTable1(TABLE& table, double date, int channel, int val1)
        {
            for (size_t i = 0; i < table.size(); i++) {
                int channel_aux = table[i]->fChan;
                // channel_aux == 0 means "all channels"
                if (channel_aux == 0 || channel == channel_aux - 1) {
                    if (fTimeStamp) {
                        table[i]->modifyZone(date, FAUSTFLOAT(val1));
                    } else {
                        table[i]->modifyZone(FAUSTFLOAT(val1));
                    }
                }
            }
        }
        
        template <typename TABLE>
        void updateTable2(TABLE& table, double date, int channel, int val1, int val2)
        {
            if (table.find(val1) != table.end()) {
                for (size_t i = 0; i < table[val1].size(); i++) {
                    int channel_aux = table[val1][i]->fChan;
                    // channel_aux == 0 means "all channels"
                    if (channel_aux == 0 || channel == channel_aux - 1) {
                        if (fTimeStamp) {
                            table[val1][i]->modifyZone(date, FAUSTFLOAT(val2));
                        } else {
                            table[val1][i]->modifyZone(FAUSTFLOAT(val2));
                        }
                    }
                }
            }
        }
    
    public:
    
        MidiUI(midi_handler* midi_handler)
        {
            fMidiHandler = midi_handler;
            fMidiHandler->addMidiIn(this);
            fTimeStamp = false;
        }
 
        virtual ~MidiUI() 
        {
            // Remove from fMidiHandler
            fMidiHandler->removeMidiIn(this);
        }

#ifdef DAISY_NO_RTTI
        virtual bool isMidiInterface() const override { return true; }
#endif
    
        bool run() { return fMidiHandler->startMidi(); }
        void stop() { fMidiHandler->stopMidi(); }
        
        void addMidiIn(midi* midi_dsp) { fMidiHandler->addMidiIn(midi_dsp); }
        void removeMidiIn(midi* midi_dsp) { fMidiHandler->removeMidiIn(midi_dsp); }
      
        // -- active widgets
        
        virtual void addButton(const char* label, FAUSTFLOAT* zone)
        {
            addGenericZone(zone, FAUSTFLOAT(0), FAUSTFLOAT(1));
        }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            addGenericZone(zone, FAUSTFLOAT(0), FAUSTFLOAT(1));
        }
        
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        {
            addGenericZone(zone, min, max);
        }

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) 
        {
            addGenericZone(zone, min, max, false);
        }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        {
            addGenericZone(zone, min, max, false);
        }

        // -- metadata declarations

        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val)
        {
            MetaDataUI::declare(zone, key, val);
            fMetaAux.push_back(std::make_pair(key, val));
        }
        
        // -- MIDI API
    
        void key(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOnTable>(fKeyTable, date, channel, note, velocity);
        }
    
        MapUI* keyOn(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOnTable>(fKeyOnTable, date, channel, note, velocity);
            // If note is in fKeyTable, handle it as a keyOn
            key(date, channel, note, velocity);
            return nullptr;
        }
        
        void keyOff(double date, int channel, int note, int velocity)
        {
            updateTable2<TKeyOffTable>(fKeyOffTable, date, channel, note, velocity);
            // If note is in fKeyTable, handle it as a keyOff with a 0 velocity
            key(date, channel, note, 0);
        }
        
        void ctrlChange(double date, int channel, int ctrl, int value)
        {
            updateTable2<TCtrlChangeTable>(fCtrlChangeTable, date, channel, ctrl, value);
        }
    
        void rpn(double date, int channel, int ctrl, int value)
        {
            if (ctrl == midi::PITCH_BEND_RANGE) {
                for (size_t i = 0; i < fPitchWheelTable.size(); i++) {
                    // channel_aux == 0 means "all channels"
                    int channel_aux = fPitchWheelTable[i]->fChan;
                    if (channel_aux == 0 || channel == channel_aux - 1) {
                        fPitchWheelTable[i]->setRange(value);
                    }
                }
            }
        }
    
        void progChange(double date, int channel, int pgm)
        {
            updateTable1<TProgChangeTable>(fProgChangeTable, date, channel, pgm);
        }
        
        void pitchWheel(double date, int channel, int wheel) 
        {
            updateTable1<TPitchWheelTable>(fPitchWheelTable, date, channel, wheel);
        }
        
        void keyPress(double date, int channel, int pitch, int press) 
        {
            updateTable2<TKeyPressTable>(fKeyPressTable, date, channel, pitch, press);
        }
        
        void chanPress(double date, int channel, int press)
        {
            updateTable1<TChanPressTable>(fChanPressTable, date, channel, press);
        }
        
        void ctrlChange14bits(double date, int channel, int ctrl, int value) {}
        
        // MIDI sync
        
        void startSync(double date)
        {
            for (size_t i = 0; i < fStartTable.size(); i++) {
                fStartTable[i]->modifyZone(date, FAUSTFLOAT(1));
            }
        }
        
        void stopSync(double date)
        {
            for (size_t i = 0; i < fStopTable.size(); i++) {
                fStopTable[i]->modifyZone(date, FAUSTFLOAT(0));
            }
        }
        
        void clock(double date)
        {
            for (size_t i = 0; i < fClockTable.size(); i++) {
                fClockTable[i]->modifyZone(date, FAUSTFLOAT(1));
            }
        }
};

#endif // FAUST_MIDIUI_H
/**************************  END  MidiUI.h **************************/
/************************** BEGIN teensy-midi.h ***************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
***************************************************************************/

#ifndef __teensy_midi__
#define __teensy_midi__

#include <cstdlib>

#include "Arduino.h"

extern usb_midi_class gUSBMIDI;

/**
 *  MIDI input/output handling for the Teensy: https://www.pjrc.com/teensy/
 */
class teensy_midi : public midi_handler {
    
    public:
        
        void processMidi()
        {
            while (gUSBMIDI.read()) {
                
                int type = gUSBMIDI.getType();       // which MIDI message, 128-255
                int channel = gUSBMIDI.getChannel(); // which MIDI channel, 0-15
                double time = (double)gUSBMIDI.Clock;
                
                switch(type) {
                    case gUSBMIDI.Clock:
                        handleClock(time);
                        break;
                    case usbMIDI.Start:
                    // We can consider start and continue as identical messages
                    case usbMIDI.Continue:
                        handleStart(time);
                        break;
                    case gUSBMIDI.Stop:
                        handleStop(time);
                        break;
                    case gUSBMIDI.ProgramChange:
                        handleProgChange(time, channel, gUSBMIDI.getData1());
                        break;
                    case gUSBMIDI.AfterTouchChannel:
                        handleAfterTouch(time, channel, gUSBMIDI.getData1());
                        break;
                    case gUSBMIDI.NoteOff:
                        handleKeyOff(time, channel, gUSBMIDI.getData1(), gUSBMIDI.getData2());
                        break;
                    case gUSBMIDI.NoteOn:
                        handleKeyOn(time, channel, gUSBMIDI.getData1(), gUSBMIDI.getData2());
                        break;
                    case gUSBMIDI.ControlChange:
                        handleCtrlChange(time, channel, gUSBMIDI.getData1(), gUSBMIDI.getData2());
                        break;
                    case gUSBMIDI.PitchBend:
                        handlePitchWheel(time, channel, gUSBMIDI.getData1(), gUSBMIDI.getData2());
                        break;
                    case gUSBMIDI.AfterTouchPoly:
                        handlePolyAfterTouch(time, channel, gUSBMIDI.getData1(), gUSBMIDI.getData2());
                        break;
                }
            }
        }
};

#endif
/**************************  END  teensy-midi.h **************************/
#endif

// for polyphonic synths
#ifdef NVOICES
/************************** BEGIN poly-dsp.h *************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
*********************************************************************/

#ifndef __poly_dsp__
#define __poly_dsp__

#include <stdio.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>
#include <ostream>
#include <sstream>
#include <vector>
#include <limits.h>
#include <float.h>
#include <assert.h>

/************************** BEGIN dsp-combiner.h **************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
************************************************************************/

#ifndef __dsp_combiner__
#define __dsp_combiner__

#include <string.h>
#include <string>
#include <assert.h>
#include <sstream>


/**
 * @file dsp-combiner.h
 * @brief DSP Combiner Library
 *
 * This library provides classes for combining DSP modules.
 * It includes classes for sequencing, parallelizing, splitting, merging, recursing, and crossfading DSP modules.
 *
 */

enum Layout { kVerticalGroup, kHorizontalGroup, kTabGroup };

/**
 * @class dsp_binary_combiner
 * @brief Base class and common code for binary combiners
 *
 * This class serves as the base class for various DSP combiners that work with two DSP modules.
 * It provides common methods for building user interfaces, allocating and deleting channels, and more.
 */
class dsp_binary_combiner : public ::dsp {

    protected:

        ::dsp* fDSP1;
        ::dsp* fDSP2;
        int fBufferSize;
        Layout fLayout;
        std::string fLabel;

        void buildUserInterfaceAux(UI* ui_interface)
        {
            switch (fLayout) {
                case kHorizontalGroup:
                    ui_interface->openHorizontalBox(fLabel.c_str());
                    fDSP1->buildUserInterface(ui_interface);
                    fDSP2->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    break;
                case kVerticalGroup:
                    ui_interface->openVerticalBox(fLabel.c_str());
                    fDSP1->buildUserInterface(ui_interface);
                    fDSP2->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    break;
                case kTabGroup:
                    ui_interface->openTabBox(fLabel.c_str());
                    ui_interface->openVerticalBox("DSP1");
                    fDSP1->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    ui_interface->openVerticalBox("DSP2");
                    fDSP2->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    ui_interface->closeBox();
                    break;
            }
        }

        FAUSTFLOAT** allocateChannels(int num)
        {
            FAUSTFLOAT** channels = new FAUSTFLOAT*[num];
            for (int chan = 0; chan < num; chan++) {
                channels[chan] = new FAUSTFLOAT[fBufferSize];
                memset(channels[chan], 0, sizeof(FAUSTFLOAT) * fBufferSize);
            }
            return channels;
        }

        void deleteChannels(FAUSTFLOAT** channels, int num)
        {
            for (int chan = 0; chan < num; chan++) {
                delete [] channels[chan];
            }
            delete [] channels;
        }

     public:

        dsp_binary_combiner(::dsp* dsp1, ::dsp* dsp2, int buffer_size, Layout layout, const std::string& label)
        :fDSP1(dsp1), fDSP2(dsp2), fBufferSize(buffer_size), fLayout(layout), fLabel(label)
        {}

        virtual ~dsp_binary_combiner()
        {
            delete fDSP1;
            delete fDSP2;
        }

        virtual int getSampleRate()
        {
            return fDSP1->getSampleRate();
        }
        virtual void init(int sample_rate)
        {
            fDSP1->init(sample_rate);
            fDSP2->init(sample_rate);
        }
        virtual void instanceInit(int sample_rate)
        {
            fDSP1->instanceInit(sample_rate);
            fDSP2->instanceInit(sample_rate);
        }
        virtual void instanceConstants(int sample_rate)
        {
            fDSP1->instanceConstants(sample_rate);
            fDSP2->instanceConstants(sample_rate);
        }

        virtual void instanceResetUserInterface()
        {
            fDSP1->instanceResetUserInterface();
            fDSP2->instanceResetUserInterface();
        }

        virtual void instanceClear()
        {
            fDSP1->instanceClear();
            fDSP2->instanceClear();
        }

        virtual void metadata(Meta* m)
        {
            fDSP1->metadata(m);
            fDSP2->metadata(m);
        }

};

/**
 * @class dsp_sequencer
 * @brief Combine two 'compatible' DSP modules in sequence
 *
 * This class allows you to combine two DSP modules in sequence.
 * It computes the first DSP module's outputs and uses them as inputs for the second DSP module.
 */
class dsp_sequencer : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Outputs;

    public:

        dsp_sequencer(::dsp* dsp1, ::dsp* dsp2,
                      int buffer_size = 4096,
                      Layout layout = Layout::kTabGroup,
                      const std::string& label = "Sequencer")
        :dsp_binary_combiner(dsp1, dsp2, buffer_size, layout, label)
        {
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs());
        }

        virtual ~dsp_sequencer()
        {
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface);
        }

        virtual dsp* clone()
        {
            return new dsp_sequencer(fDSP1->clone(), fDSP2->clone(), fBufferSize, fLayout, fLabel);
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, fDSP1Outputs);
            fDSP2->compute(count, fDSP1Outputs, outputs);
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

/**
 * @class dsp_parallelizer
 * @brief Combine two DSP modules in parallel
 *
 * This class combines two DSP modules in parallel.
 * It computes both DSP modules separately and combines their outputs.
 */
class dsp_parallelizer : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP2Inputs;
        FAUSTFLOAT** fDSP2Outputs;

    public:

        dsp_parallelizer(::dsp* dsp1, ::dsp* dsp2,
                     int buffer_size = 4096,
                     Layout layout = Layout::kTabGroup,
                     const std::string& label = "Parallelizer")
        :dsp_binary_combiner(dsp1, dsp2, buffer_size, layout, label)
        {
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
            fDSP2Outputs = new FAUSTFLOAT*[fDSP2->getNumOutputs()];
        }

        virtual ~dsp_parallelizer()
        {
            delete [] fDSP2Inputs;
            delete [] fDSP2Outputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs() + fDSP2->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP1->getNumOutputs() + fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface);
        }

        virtual dsp* clone()
        {
            return new dsp_parallelizer(fDSP1->clone(), fDSP2->clone(), fBufferSize, fLayout, fLabel);
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, outputs);

            // Shift inputs/outputs channels for fDSP2
            for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                fDSP2Inputs[chan] = inputs[fDSP1->getNumInputs() + chan];
            }
            for (int chan = 0; chan < fDSP2->getNumOutputs(); chan++) {
                fDSP2Outputs[chan] = outputs[fDSP1->getNumOutputs() + chan];
            }

            fDSP2->compute(count, fDSP2Inputs, fDSP2Outputs);
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

/**
 * @class dsp_splitter
 * @brief Combine two 'compatible' DSP modules in a splitter
 *
 * This class combines two DSP modules in a splitter configuration.
 * The outputs of the first DSP module are connected to the inputs of the second DSP module.
 */
class dsp_splitter : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Outputs;
        FAUSTFLOAT** fDSP2Inputs;

    public:

        dsp_splitter(::dsp* dsp1, ::dsp* dsp2,
                     int buffer_size = 4096,
                     Layout layout = Layout::kTabGroup,
                     const std::string& label = "Splitter")
        :dsp_binary_combiner(dsp1, dsp2, buffer_size, layout, label)
        {
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs());
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
        }

        virtual ~dsp_splitter()
        {
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            delete [] fDSP2Inputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface);
        }

        virtual ::dsp* clone()
        {
            return new dsp_splitter(fDSP1->clone(), fDSP2->clone(), fBufferSize, fLayout, fLabel);
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, inputs, fDSP1Outputs);

            for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                 fDSP2Inputs[chan] = fDSP1Outputs[chan % fDSP1->getNumOutputs()];
            }

            fDSP2->compute(count, fDSP2Inputs, outputs);
        }
};

/**
 * @class dsp_merger
 * @brief Combine two 'compatible' DSP modules in a merger
 *
 * This class combines two DSP modules in a merger configuration.
 * The outputs of the first DSP module are combined with the inputs of the second DSP module.
 */
class dsp_merger : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Inputs;
        FAUSTFLOAT** fDSP1Outputs;
        FAUSTFLOAT** fDSP2Inputs;

        void mix(int count, FAUSTFLOAT* dst, FAUSTFLOAT* src)
        {
            for (int frame = 0; frame < count; frame++) {
                dst[frame] += src[frame];
            }
        }

    public:

        dsp_merger(dsp* dsp1, dsp* dsp2,
                   int buffer_size = 4096,
                   Layout layout = Layout::kTabGroup,
                   const std::string& label = "Merger")
        :dsp_binary_combiner(dsp1, dsp2, buffer_size, layout, label)
        {
            fDSP1Inputs = allocateChannels(fDSP1->getNumInputs());
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs());
            fDSP2Inputs = new FAUSTFLOAT*[fDSP2->getNumInputs()];
        }

        virtual ~dsp_merger()
        {
            deleteChannels(fDSP1Inputs, fDSP1->getNumInputs());
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            delete [] fDSP2Inputs;
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP2->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface);
        }

        virtual ::dsp* clone()
        {
            return new dsp_merger(fDSP1->clone(), fDSP2->clone(), fBufferSize, fLayout, fLabel);
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            fDSP1->compute(count, fDSP1Inputs, fDSP1Outputs);

            memset(fDSP2Inputs, 0, sizeof(FAUSTFLOAT*) * fDSP2->getNumInputs());

            for (int chan = 0; chan < fDSP1->getNumOutputs(); chan++) {
                int mchan = chan % fDSP2->getNumInputs();
                if (fDSP2Inputs[mchan]) {
                    mix(count, fDSP2Inputs[mchan], fDSP1Outputs[chan]);
                } else {
                    fDSP2Inputs[mchan] = fDSP1Outputs[chan];
                }
            }

            fDSP2->compute(count, fDSP2Inputs, outputs);
        }
};

/**
 * @class dsp_recursiver
 * @brief Combine two 'compatible' DSP modules in a recursive way
 *
 * This class recursively combines two DSP modules.
 * The outputs of each module are fed as inputs to the other module in a recursive manner.
 */
class dsp_recursiver : public dsp_binary_combiner {

    private:

        FAUSTFLOAT** fDSP1Inputs;
        FAUSTFLOAT** fDSP1Outputs;

        FAUSTFLOAT** fDSP2Inputs;
        FAUSTFLOAT** fDSP2Outputs;

    public:

        dsp_recursiver(::dsp* dsp1, ::dsp* dsp2,
                       Layout layout = Layout::kTabGroup,
                       const std::string& label = "Recursiver")
        :dsp_binary_combiner(dsp1, dsp2, 1, layout, label)
        {
            fDSP1Inputs = allocateChannels(fDSP1->getNumInputs());
            fDSP1Outputs = allocateChannels(fDSP1->getNumOutputs());
            fDSP2Inputs = allocateChannels(fDSP2->getNumInputs());
            fDSP2Outputs = allocateChannels(fDSP2->getNumOutputs());
        }

        virtual ~dsp_recursiver()
        {
            deleteChannels(fDSP1Inputs, fDSP1->getNumInputs());
            deleteChannels(fDSP1Outputs, fDSP1->getNumOutputs());
            deleteChannels(fDSP2Inputs, fDSP2->getNumInputs());
            deleteChannels(fDSP2Outputs, fDSP2->getNumOutputs());
        }

        virtual int getNumInputs() { return fDSP1->getNumInputs() - fDSP2->getNumOutputs(); }
        virtual int getNumOutputs() { return fDSP1->getNumOutputs(); }

        virtual void buildUserInterface(UI* ui_interface)
        {
            buildUserInterfaceAux(ui_interface);
        }

        virtual dsp* clone()
        {
            return new dsp_recursiver(fDSP1->clone(), fDSP2->clone(), fLayout, fLabel);
        }

        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            for (int frame = 0; (frame < count); frame++) {

                for (int chan = 0; chan < fDSP2->getNumOutputs(); chan++) {
                    fDSP1Inputs[chan][0] = fDSP2Outputs[chan][0];
                }

                for (int chan = 0; chan < fDSP1->getNumInputs() - fDSP2->getNumOutputs(); chan++) {
                    fDSP1Inputs[chan + fDSP2->getNumOutputs()][0] = inputs[chan][frame];
                }

                fDSP1->compute(1, fDSP1Inputs, fDSP1Outputs);

                for (int chan = 0; chan < fDSP1->getNumOutputs(); chan++) {
                    outputs[chan][frame] = fDSP1Outputs[chan][0];
                }

                for (int chan = 0; chan < fDSP2->getNumInputs(); chan++) {
                    fDSP2Inputs[chan][0] = fDSP1Outputs[chan][0];
                }

                fDSP2->compute(1, fDSP2Inputs, fDSP2Outputs);
            }
        }

        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }

};

/**
 * @class dsp_crossfader
 * @brief Crossfade between two DSP modules
 *
 * This class allows you to crossfade between two DSP modules.
 * The crossfade parameter (as a slider) controls the mix between the two modules' outputs.
 * When Crossfade = 1, the first DSP only is computed, when Crossfade = 0,
 * the second DSP only is computed, otherwise both DSPs are computed and mixed.
 */
class dsp_crossfader: public dsp_binary_combiner {

    private:
    
        FAUSTFLOAT fCrossfade;
        FAUSTFLOAT** fDSPOutputs1;
        FAUSTFLOAT** fDSPOutputs2;
    
    public:
    
        dsp_crossfader(::dsp* dsp1, ::dsp* dsp2,
                       Layout layout = Layout::kTabGroup,
                       const std::string& label = "Crossfade")
        :dsp_binary_combiner(dsp1, dsp2, 4096, layout, label),fCrossfade(FAUSTFLOAT(0.5))
        {
            fDSPOutputs1 = allocateChannels(fDSP1->getNumOutputs());
            fDSPOutputs2 = allocateChannels(fDSP1->getNumOutputs());
        }
    
        virtual ~dsp_crossfader()
        {
            deleteChannels(fDSPOutputs1, fDSP1->getNumInputs());
            deleteChannels(fDSPOutputs2, fDSP1->getNumOutputs());
        }
    
        virtual int getNumInputs() { return fDSP1->getNumInputs(); }
        virtual int getNumOutputs() { return fDSP1->getNumOutputs(); }

        void buildUserInterface(UI* ui_interface)
        {
            switch (fLayout) {
                case kHorizontalGroup:
                    ui_interface->openHorizontalBox(fLabel.c_str());
                    ui_interface->addHorizontalSlider("Crossfade", &fCrossfade, FAUSTFLOAT(0.5), FAUSTFLOAT(0), FAUSTFLOAT(1), FAUSTFLOAT(0.01));
                    fDSP1->buildUserInterface(ui_interface);
                    fDSP2->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    break;
                case kVerticalGroup:
                    ui_interface->openVerticalBox(fLabel.c_str());
                    ui_interface->addHorizontalSlider("Crossfade", &fCrossfade, FAUSTFLOAT(0.5), FAUSTFLOAT(0), FAUSTFLOAT(1), FAUSTFLOAT(0.01));
                    fDSP1->buildUserInterface(ui_interface);
                    fDSP2->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    break;
                case kTabGroup:
                    ui_interface->openTabBox(fLabel.c_str());
                    ui_interface->openVerticalBox("Crossfade");
                    ui_interface->addHorizontalSlider("Crossfade", &fCrossfade, FAUSTFLOAT(0.5), FAUSTFLOAT(0), FAUSTFLOAT(1), FAUSTFLOAT(0.01));
                    ui_interface->closeBox();
                    ui_interface->openVerticalBox("DSP1");
                    fDSP1->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    ui_interface->openVerticalBox("DSP2");
                    fDSP2->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                    ui_interface->closeBox();
                    break;
            }
        }
    
        virtual ::dsp* clone()
        {
            return new dsp_crossfader(fDSP1->clone(), fDSP2->clone(), fLayout, fLabel);
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            if (fCrossfade == FAUSTFLOAT(1)) {
                fDSP1->compute(count, inputs, outputs);
            } else if (fCrossfade == FAUSTFLOAT(0)) {
                fDSP2->compute(count, inputs, outputs);
            } else {
                // Compute each effect
                fDSP1->compute(count, inputs, fDSPOutputs1);
                fDSP2->compute(count, inputs, fDSPOutputs2);
                // Mix between the two effects
                FAUSTFLOAT gain1 = fCrossfade;
                FAUSTFLOAT gain2 = FAUSTFLOAT(1) - gain1;
                for (int frame = 0; (frame < count); frame++) {
                    for (int chan = 0; chan < fDSP1->getNumOutputs(); chan++) {
                        outputs[chan][frame] = fDSPOutputs1[chan][frame] * gain1 + fDSPOutputs2[chan][frame] * gain2;
                    }
                }
            }
        }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

#ifndef __dsp_algebra_api__
#define __dsp_algebra_api__

/**
 * DSP algebra API allowing to combine DSPs using the 5 operators Faust block algebra and an additional crossfader combiner.
 * The two arguments GUI are composed in a group, either kVerticalGroup, kHorizontalGroup or kTabGroup with a label.
 *
 * Each operation takes two DSP and a optional layout and label parameters, returns the combined DSPs,
 * or null if failure with an error message.
 * 
 * It includes methods to create sequencers, parallelizers, splitters, mergers, recursivers, and crossfaders.
 */

/**
 * Create a DSP Sequencer
 *
 * This method creates a DSP Sequencer, which combines two DSP modules in a sequencer configuration.
 * The outputs of the first DSP module are connected to the inputs of the second DSP module.
 *
 * @param dsp1 The first DSP module to combine
 * @param dsp2 The second DSP module to combine
 * @param error A reference to a string to store error messages (if any)
 * @param layout The layout for the combined user interface (default: kTabGroup)
 * @param label The label for the combiner (default: "Sequencer")
 * @return A pointer to the created DSP Sequencer, or nullptr if an error occurs
 */
static ::dsp* createDSPSequencer(::dsp* dsp1, ::dsp* dsp2,
                                 std::string& error,
                                 Layout layout = Layout::kTabGroup,
                                 const std::string& label = "Sequencer")
{
    if (dsp1->getNumOutputs() != dsp2->getNumInputs()) {
        std::stringstream error_aux;
        error_aux << "Connection error in dsp_sequencer : the number of outputs ("
                  << dsp1->getNumOutputs() << ") of A "
                  << "must be equal to the number of inputs (" << dsp2->getNumInputs() << ") of B" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else {
        return new dsp_sequencer(dsp1, dsp2, 4096, layout, label);
    }
}

/**
 * Create a DSP Parallelizer
 *
 * This method creates a DSP Parallelizer, which combines two DSP modules in parallel.
 * The resulting DSP module computes both input modules separately and combines their outputs.
 *
 * @param dsp1 The first DSP module to combine
 * @param dsp2 The second DSP module to combine
 * @param error A reference to a string to store error messages (if any)
 * @param layout The layout for the combined user interface (default: kTabGroup)
 * @param label The label for the combiner (default: "Parallelizer")
 * @return A pointer to the created DSP Parallelizer, or nullptr if an error occurs
 */
static ::dsp* createDSPParallelizer(::dsp* dsp1, dsp* dsp2,
                                    std::string& error,
                                    Layout layout = Layout::kTabGroup,
                                    const std::string& label = "Parallelizer")
{
    return new dsp_parallelizer(dsp1, dsp2, 4096, layout, label);
}

/**
 * Create a DSP Splitter
 *
 * This method creates a DSP Splitter, which combines two 'compatible' DSP modules in a splitter configuration.
 * The outputs of the first DSP module are connected to the inputs of the second DSP module.
 *
 * @param dsp1 The first DSP module to combine
 * @param dsp2 The second DSP module to combine
 * @param error A reference to a string to store error messages (if any)
 * @param layout The layout for the combined user interface (default: kTabGroup)
 * @param label The label for the combiner (default: "Splitter")
 * @return A pointer to the created DSP Splitter, or nullptr if an error occurs
 */
static ::dsp* createDSPSplitter(::dsp* dsp1, ::dsp* dsp2, std::string& error, Layout layout = Layout::kTabGroup, const std::string& label = "Splitter")
{
    if (dsp1->getNumOutputs() == 0) {
        error = "Connection error in dsp_splitter : the first expression has no outputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() == 0) {
        error = "Connection error in dsp_splitter : the second expression has no inputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() % dsp1->getNumOutputs() != 0) {
        std::stringstream error_aux;
        error_aux << "Connection error in dsp_splitter : the number of outputs (" << dsp1->getNumOutputs()
                  << ") of the first expression should be a divisor of the number of inputs ("
                  << dsp2->getNumInputs()
                  << ") of the second expression" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else if (dsp2->getNumInputs() == dsp1->getNumOutputs()) {
        return new dsp_sequencer(dsp1, dsp2, 4096, layout, label);
    } else {
        return new dsp_splitter(dsp1, dsp2, 4096, layout, label);
    }
}

/**
 * Create a DSP Merger
 *
 * This method creates a DSP Merger, which combines two 'compatible' DSP modules in a merger configuration.
 * The outputs of the first DSP module are combined with the inputs of the second DSP module.
 *
 * @param dsp1 The first DSP module to combine
 * @param dsp2 The second DSP module to combine
 * @param error A reference to a string to store error messages (if any)
 * @param layout The layout for the combined user interface (default: kTabGroup)
 * @param label The label for the combiner (default: "Merger")
 * @return A pointer to the created DSP Merger, or nullptr if an error occurs
 */
static ::dsp* createDSPMerger(::dsp* dsp1, ::dsp* dsp2,
                              std::string& error,
                              Layout layout = Layout::kTabGroup,
                              const std::string& label = "Merger")
{
    if (dsp1->getNumOutputs() == 0) {
        error = "Connection error in dsp_merger : the first expression has no outputs\n";
        return nullptr;
    } else if (dsp2->getNumInputs() == 0) {
        error = "Connection error in dsp_merger : the second expression has no inputs\n";
        return nullptr;
    } else if (dsp1->getNumOutputs() % dsp2->getNumInputs() != 0) {
        std::stringstream error_aux;
        error_aux << "Connection error in dsp_merger : the number of outputs (" << dsp1->getNumOutputs()
                  << ") of the first expression should be a multiple of the number of inputs ("
                  << dsp2->getNumInputs()
                  << ") of the second expression" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else if (dsp2->getNumInputs() == dsp1->getNumOutputs()) {
        return new dsp_sequencer(dsp1, dsp2, 4096, layout, label);
    } else {
        return new dsp_merger(dsp1, dsp2, 4096, layout, label);
    }
}

/**
 * Create a DSP Recursiver
 *
 * This method creates a DSP Recursiver, which combines two 'compatible' DSP modules in a recursive way.
 * The outputs of each module are fed as inputs to the other module in a recursive manner.
 *
 * @param dsp1 The first DSP module to combine
 * @param dsp2 The second DSP module to combine
 * @param error A reference to a string to store error messages (if any)
 * @param layout The layout for the combined user interface (default: kTabGroup)
 * @param label The label for the combiner (default: "Recursiver")
 * @return A pointer to the created DSP Recursiver, or nullptr if an error occurs
 */
static ::dsp* createDSPRecursiver(::dsp* dsp1, ::dsp* dsp2,
                                  std::string& error,
                                  Layout layout = Layout::kTabGroup,
                                  const std::string& label = "Recursiver")
{
    if ((dsp2->getNumInputs() > dsp1->getNumOutputs()) || (dsp2->getNumOutputs() > dsp1->getNumInputs())) {
        std::stringstream error_aux;
        error_aux << "Connection error in : dsp_recursiver" << std::endl;
        if (dsp2->getNumInputs() > dsp1->getNumOutputs()) {
            error_aux << "The number of outputs " << dsp1->getNumOutputs()
                      << " of the first expression should be greater or equal to the number of inputs ("
                      << dsp2->getNumInputs()
                      << ") of the second expression" << std::endl;
        }
        if (dsp2->getNumOutputs() > dsp1->getNumInputs()) {
            error_aux << "The number of inputs " << dsp1->getNumInputs()
                      << " of the first expression should be greater or equal to the number of outputs ("
                      << dsp2->getNumOutputs()
                      << ") of the second expression" << std::endl;
        }
        error = error_aux.str();
        return nullptr;
    } else {
        return new dsp_recursiver(dsp1, dsp2, layout, label);
    }
}

/**
 * Create a DSP Crossfader
 *
 * This method creates a DSP Crossfader, which allows you to crossfade between two DSP modules.
 * The crossfade parameter (as a slider) controls the mix between the two modules outputs.
 * When Crossfade = 1, the first DSP only is computed, when Crossfade = 0,
 * the second DSP only is computed, otherwise both DSPs are computed and mixed.
 *
 * @param dsp1 The first DSP module to combine
 * @param dsp2 The second DSP module to combine
 * @param error A reference to a string to store error messages (if any)
 * @param layout The layout for the combined user interface (default: kTabGroup)
 * @param label The label for the crossfade slider (default: "Crossfade")
 * @return A pointer to the created DSP Crossfader, or nullptr if an error occurs
 */
static ::dsp* createDSPCrossfader(::dsp* dsp1, ::dsp* dsp2,
                                  std::string& error,
                                  Layout layout = Layout::kTabGroup,
                                  const std::string& label = "Crossfade")
{
    if (dsp1->getNumInputs() != dsp2->getNumInputs()) {
        std::stringstream error_aux;
        error_aux << "Error in dsp_crossfader : the number of inputs ("
        << dsp1->getNumInputs() << ") of A "
        << "must be equal to the number of inputs (" << dsp2->getNumInputs() << ") of B" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else if (dsp1->getNumOutputs() != dsp2->getNumOutputs()) {
        std::stringstream error_aux;
        error_aux << "Error in dsp_crossfader : the number of outputs ("
        << dsp1->getNumOutputs() << ") of A "
        << "must be equal to the number of outputs (" << dsp2->getNumOutputs() << ") of B" << std::endl;
        error = error_aux.str();
        return nullptr;
    } else {
        return new dsp_crossfader(dsp1, dsp2, layout, label);
    }
}

#endif

#endif
/************************** END dsp-combiner.h **************************/
/************************** BEGIN dsp-adapter.h *************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __dsp_adapter__
#define __dsp_adapter__

#ifndef _WIN32
#include <alloca.h>
#endif
#include <string.h>
#include <cmath>
#include <assert.h>
#include <stdio.h>


// Adapts a DSP for a different number of inputs/outputs
class dsp_adapter : public decorator_dsp {
    
    private:
    
        FAUSTFLOAT** fAdaptedInputs;
        FAUSTFLOAT** fAdaptedOutputs;
        int fHWInputs;
        int fHWOutputs;
        int fDSPInputs;
        int fDSPOutputs;
    
        int fBufferSize;
        bool fDelete;
    
        void adaptBuffers(FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            for (int i = 0; i < fHWInputs; i++) {
                fAdaptedInputs[i] = inputs[i];
            }
            for (int i = 0; i < fHWOutputs; i++) {
                fAdaptedOutputs[i] = outputs[i];
            }
        }
    
    public:
    
        dsp_adapter(::dsp* dsp, int hw_inputs, int hw_outputs, int buffer_size, bool to_delete = true):decorator_dsp(dsp)
        {
            fHWInputs = hw_inputs;
            fHWOutputs = hw_outputs;
            fDSPInputs = dsp->getNumInputs();
            fDSPOutputs = dsp->getNumOutputs();
            fBufferSize = buffer_size;
            fDelete = to_delete;
            
            const int input_slots = std::max<int>(fDSPInputs, fHWInputs);
            fAdaptedInputs = new FAUSTFLOAT*[input_slots];
            const int extra_inputs = std::max(0, fDSPInputs - fHWInputs);
            for (int i = 0; i < extra_inputs; i++) {
                fAdaptedInputs[i + fHWInputs] = new FAUSTFLOAT[buffer_size];
                memset(fAdaptedInputs[i + fHWInputs], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
            
            const int output_slots = std::max<int>(fDSPOutputs, fHWOutputs);
            fAdaptedOutputs = new FAUSTFLOAT*[output_slots];
            const int extra_outputs = std::max(0, fDSPOutputs - fHWOutputs);
            for (int i = 0; i < extra_outputs; i++) {
                fAdaptedOutputs[i + fHWOutputs] = new FAUSTFLOAT[buffer_size];
                memset(fAdaptedOutputs[i + fHWOutputs], 0, sizeof(FAUSTFLOAT) * buffer_size);
            }
        }
    
        virtual ~dsp_adapter()
        {
            for (int i = 0; i < fDSPInputs - fHWInputs; i++) {
                delete [] fAdaptedInputs[i + fHWInputs];
            }
            delete [] fAdaptedInputs;
            
            for (int i = 0; i < fDSPOutputs - fHWOutputs; i++) {
                delete [] fAdaptedOutputs[i + fHWOutputs];
            }
            delete [] fAdaptedOutputs;
        
            // Decorator should not delete the decorated fDSP
            if (!fDelete) fDSP = nullptr;
        }
    
        virtual int getNumInputs() { return fHWInputs; }
        virtual int getNumOutputs() { return fHWOutputs; }
    
        virtual dsp_adapter* clone() { return new dsp_adapter(fDSP->clone(), fHWInputs, fHWOutputs, fBufferSize); }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptBuffers(inputs, outputs);
            fDSP->compute(date_usec, count, fAdaptedInputs, fAdaptedOutputs);
        }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            adaptBuffers(inputs, outputs);
            fDSP->compute(count, fAdaptedInputs, fAdaptedOutputs);
        }
};

// Adapts a DSP for a different sample size
template <typename REAL_INT, typename REAL_EXT>
class dsp_sample_adapter : public decorator_dsp {
    
    private:
    
        REAL_INT** fAdaptedInputs;
        REAL_INT** fAdaptedOutputs;
    
        void adaptInputBuffers(int count, FAUSTFLOAT** inputs)
        {
            for (int chan = 0; chan < fDSP->getNumInputs(); chan++) {
                for (int frame = 0; frame < count; frame++) {
                    fAdaptedInputs[chan][frame] = REAL_INT(reinterpret_cast<REAL_EXT**>(inputs)[chan][frame]);
                }
            }
        }
    
        void adaptOutputsBuffers(int count, FAUSTFLOAT** outputs)
        {
            for (int chan = 0; chan < fDSP->getNumOutputs(); chan++) {
                for (int frame = 0; frame < count; frame++) {
                    reinterpret_cast<REAL_EXT**>(outputs)[chan][frame] = REAL_EXT(fAdaptedOutputs[chan][frame]);
                }
            }
        }
    
    public:
    
        dsp_sample_adapter(::dsp* dsp):decorator_dsp(dsp)
        {
            fAdaptedInputs = new REAL_INT*[dsp->getNumInputs()];
            for (int i = 0; i < dsp->getNumInputs(); i++) {
                fAdaptedInputs[i] = new REAL_INT[4096];
            }
            
            fAdaptedOutputs = new REAL_INT*[dsp->getNumOutputs()];
            for (int i = 0; i < dsp->getNumOutputs(); i++) {
                fAdaptedOutputs[i] = new REAL_INT[4096];
            }
        }
    
        virtual ~dsp_sample_adapter()
        {
            for (int i = 0; i < fDSP->getNumInputs(); i++) {
                delete [] fAdaptedInputs[i];
            }
            delete [] fAdaptedInputs;
            
            for (int i = 0; i < fDSP->getNumOutputs(); i++) {
                delete [] fAdaptedOutputs[i];
            }
            delete [] fAdaptedOutputs;
        }
    
        virtual dsp_sample_adapter* clone() { return new dsp_sample_adapter(fDSP->clone()); }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            assert(count <= 4096);
            adaptInputBuffers(count, inputs);
            // DSP base class uses FAUSTFLOAT** type, so reinterpret_cast has to be used even if the real DSP uses REAL_INT
            fDSP->compute(count, reinterpret_cast<FAUSTFLOAT**>(fAdaptedInputs), reinterpret_cast<FAUSTFLOAT**>(fAdaptedOutputs));
            adaptOutputsBuffers(count, outputs);
        }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            assert(count <= 4096);
            adaptInputBuffers(count, inputs);
            // DSP base class uses FAUSTFLOAT** type, so reinterpret_cast has to be used even if the real DSP uses REAL_INT
            fDSP->compute(date_usec, count, reinterpret_cast<FAUSTFLOAT**>(fAdaptedInputs), reinterpret_cast<FAUSTFLOAT**>(fAdaptedOutputs));
            adaptOutputsBuffers(count, outputs);
        }
};

// Template used to specialize double parameters expressed as NUM/DENOM
template <int NUM, int DENOM>
struct Double {
    static constexpr double value() { return double(NUM)/double(DENOM); }
};

// Base class for filters
template <class fVslider0, int fVslider1>
struct Filter {
    inline int getFactor() { return fVslider1; }
};

// Identity filter: copy input to output
template <class fVslider0, int fVslider1>
struct Identity : public Filter<fVslider0, fVslider1> {
    inline int getFactor() { return fVslider1; }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        memcpy(output0, input0, count * sizeof(FAUSTFLOAT));
    }
};

// Generated with process = fi.lowpass(3, ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass3 : public Filter<fVslider0, fVslider1> {
    
    REAL fVec0[2];
    REAL fRec1[2];
    REAL fRec0[3];
    
    inline REAL LowPass3_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass3()
    {
        for (int l0 = 0; (l0 < 2); l0 = (l0 + 1)) {
            fVec0[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
            fRec1[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 1.0000000000000002) / fSlow0) + 1.0));
        REAL fSlow3 = (1.0 / (fSlow1 + 1.0));
        REAL fSlow4 = (1.0 - fSlow1);
        REAL fSlow5 = (((fSlow1 + -1.0000000000000002) / fSlow0) + 1.0);
        REAL fSlow6 = (2.0 * (1.0 - (1.0 / LowPass3_faustpower2_f(fSlow0))));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            REAL fTemp0 = REAL(input0[i]);
            fVec0[0] = fTemp0;
            fRec1[0] = (0.0 - (fSlow3 * ((fSlow4 * fRec1[1]) - (fTemp0 + fVec0[1]))));
            fRec0[0] = (fRec1[0] - (fSlow2 * ((fSlow5 * fRec0[2]) + (fSlow6 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (fRec0[2] + (fRec0[0] + (2.0 * fRec0[1])))));
            fVec0[1] = fVec0[0];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass(4, ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass4 : public Filter<fVslider0, fVslider1> {
    
    REAL fRec1[3];
    REAL fRec0[3];
    
    inline REAL LowPass4_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass4()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec1[l0] = 0.0f;
        }
        for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
            fRec0[l1] = 0.0f;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 0.76536686473017945) / fSlow0) + 1.0));
        REAL fSlow3 = (1.0 / (((fSlow1 + 1.8477590650225735) / fSlow0) + 1.0));
        REAL fSlow4 = (((fSlow1 + -1.8477590650225735) / fSlow0) + 1.0);
        REAL fSlow5 = (2.0 * (1.0 - (1.0 / LowPass4_faustpower2_f(fSlow0))));
        REAL fSlow6 = (((fSlow1 + -0.76536686473017945) / fSlow0) + 1.0);
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec1[0] = (REAL(input0[i]) - (fSlow3 * ((fSlow4 * fRec1[2]) + (fSlow5 * fRec1[1]))));
            fRec0[0] = ((fSlow3 * (fRec1[2] + (fRec1[0] + (2.0 * fRec1[1])))) - (fSlow2 * ((fSlow6 * fRec0[2]) + (fSlow5 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (fRec0[2] + (fRec0[0] + (2.0 * fRec0[1])))));
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass3e(ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass3e : public Filter<fVslider0, fVslider1> {
    
    REAL fRec1[3];
    REAL fVec0[2];
    REAL fRec0[2];
    
    inline REAL LowPass3e_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass3e()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec1[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 2); l1 = (l1 + 1)) {
            fVec0[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 2); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (fSlow1 + 0.82244590899881598));
        REAL fSlow3 = (0.82244590899881598 - fSlow1);
        REAL fSlow4 = (1.0 / (((fSlow1 + 0.80263676416103003) / fSlow0) + 1.4122708937742039));
        REAL fSlow5 = LowPass3e_faustpower2_f(fSlow0);
        REAL fSlow6 = (0.019809144837788999 / fSlow5);
        REAL fSlow7 = (fSlow6 + 1.1615164189826961);
        REAL fSlow8 = (((fSlow1 + -0.80263676416103003) / fSlow0) + 1.4122708937742039);
        REAL fSlow9 = (2.0 * (1.4122708937742039 - (1.0 / fSlow5)));
        REAL fSlow10 = (2.0 * (1.1615164189826961 - fSlow6));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec1[0] = (REAL(input0[i]) - (fSlow4 * ((fSlow8 * fRec1[2]) + (fSlow9 * fRec1[1]))));
            REAL fTemp0 = (fSlow4 * (((fSlow7 * fRec1[0]) + (fSlow10 * fRec1[1])) + (fSlow7 * fRec1[2])));
            fVec0[0] = fTemp0;
            fRec0[0] = (0.0 - (fSlow2 * ((fSlow3 * fRec0[1]) - (fTemp0 + fVec0[1]))));
            output0[i] = FAUSTFLOAT(fRec0[0]);
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fVec0[1] = fVec0[0];
            fRec0[1] = fRec0[0];
        }
    }
};

// Generated with process = fi.lowpass6e(ma.SR*hslider("FCFactor", 0.4, 0.4, 0.5, 0.01)/hslider("Factor", 2, 2, 8, 1));
template <class fVslider0, int fVslider1, typename REAL>
struct LowPass6e : public Filter<fVslider0, fVslider1> {
    
    REAL fRec2[3];
    REAL fRec1[3];
    REAL fRec0[3];
    
    inline REAL LowPass6e_faustpower2_f(REAL value)
    {
        return (value * value);
    }
    
    LowPass6e()
    {
        for (int l0 = 0; (l0 < 3); l0 = (l0 + 1)) {
            fRec2[l0] = 0.0;
        }
        for (int l1 = 0; (l1 < 3); l1 = (l1 + 1)) {
            fRec1[l1] = 0.0;
        }
        for (int l2 = 0; (l2 < 3); l2 = (l2 + 1)) {
            fRec0[l2] = 0.0;
        }
    }
    
    inline void compute(int count, FAUSTFLOAT* input0, FAUSTFLOAT* output0)
    {
        // Computed at template specialization time
        REAL fSlow0 = std::tan((3.1415926535897931 * (REAL(fVslider0::value()) / REAL(fVslider1))));
        REAL fSlow1 = (1.0 / fSlow0);
        REAL fSlow2 = (1.0 / (((fSlow1 + 0.16840487111358901) / fSlow0) + 1.0693584077073119));
        REAL fSlow3 = LowPass6e_faustpower2_f(fSlow0);
        REAL fSlow4 = (1.0 / fSlow3);
        REAL fSlow5 = (fSlow4 + 53.536152954556727);
        REAL fSlow6 = (1.0 / (((fSlow1 + 0.51247864188914105) / fSlow0) + 0.68962136448467504));
        REAL fSlow7 = (fSlow4 + 7.6217312988706034);
        REAL fSlow8 = (1.0 / (((fSlow1 + 0.78241304682164503) / fSlow0) + 0.24529150870616001));
        REAL fSlow9 = (9.9999997054999994e-05 / fSlow3);
        REAL fSlow10 = (fSlow9 + 0.00043322720055500002);
        REAL fSlow11 = (((fSlow1 + -0.78241304682164503) / fSlow0) + 0.24529150870616001);
        REAL fSlow12 = (2.0 * (0.24529150870616001 - fSlow4));
        REAL fSlow13 = (2.0 * (0.00043322720055500002 - fSlow9));
        REAL fSlow14 = (((fSlow1 + -0.51247864188914105) / fSlow0) + 0.68962136448467504);
        REAL fSlow15 = (2.0 * (0.68962136448467504 - fSlow4));
        REAL fSlow16 = (2.0 * (7.6217312988706034 - fSlow4));
        REAL fSlow17 = (((fSlow1 + -0.16840487111358901) / fSlow0) + 1.0693584077073119);
        REAL fSlow18 = (2.0 * (1.0693584077073119 - fSlow4));
        REAL fSlow19 = (2.0 * (53.536152954556727 - fSlow4));
        // Computed at runtime
        for (int i = 0; (i < count); i = (i + 1)) {
            fRec2[0] = (REAL(input0[i]) - (fSlow8 * ((fSlow11 * fRec2[2]) + (fSlow12 * fRec2[1]))));
            fRec1[0] = ((fSlow8 * (((fSlow10 * fRec2[0]) + (fSlow13 * fRec2[1])) + (fSlow10 * fRec2[2]))) - (fSlow6 * ((fSlow14 * fRec1[2]) + (fSlow15 * fRec1[1]))));
            fRec0[0] = ((fSlow6 * (((fSlow7 * fRec1[0]) + (fSlow16 * fRec1[1])) + (fSlow7 * fRec1[2]))) - (fSlow2 * ((fSlow17 * fRec0[2]) + (fSlow18 * fRec0[1]))));
            output0[i] = FAUSTFLOAT((fSlow2 * (((fSlow5 * fRec0[0]) + (fSlow19 * fRec0[1])) + (fSlow5 * fRec0[2]))));
            fRec2[2] = fRec2[1];
            fRec2[1] = fRec2[0];
            fRec1[2] = fRec1[1];
            fRec1[1] = fRec1[0];
            fRec0[2] = fRec0[1];
            fRec0[1] = fRec0[0];
        }
    }
};

// A "si.bus(N)" like hard-coded class
struct dsp_bus : public ::dsp {
    
    int fChannels;
    int fSampleRate;
    
    dsp_bus(int channels):fChannels(channels), fSampleRate(-1)
    {}
    
    virtual int getNumInputs() { return fChannels; }
    virtual int getNumOutputs() { return fChannels; }
    
    virtual int getSampleRate() { return fSampleRate; }
    
    virtual void buildUserInterface(UI* ui_interface) {}
    virtual void init(int sample_rate)
    {
        //classInit(sample_rate);
        instanceInit(sample_rate);
    }
    
    virtual void instanceInit(int sample_rate)
    {
        fSampleRate = sample_rate;
        instanceConstants(sample_rate);
        instanceResetUserInterface();
        instanceClear();
    }
    
    virtual void instanceConstants(int sample_rate) {}
    virtual void instanceResetUserInterface() {}
    virtual void instanceClear() {}
    
    virtual ::dsp* clone() { return new dsp_bus(fChannels); }
    
    virtual void metadata(Meta* m) {}
    
    virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        for (int chan = 0; chan < fChannels; chan++) {
            memcpy(outputs[chan], inputs[chan], sizeof(FAUSTFLOAT) * count);
        }
    }
    
    virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        compute(count, inputs, outputs);
    }
    
};

// Base class for sample-rate adapter
template <typename FILTER>
class sr_sampler : public decorator_dsp {
    
    protected:
    
        std::vector<FILTER> fInputLowPass;
        std::vector<FILTER> fOutputLowPass;
    
        inline int getFactor() { return this->fOutputLowPass[0].getFactor(); }
    
    public:
    
        sr_sampler(::dsp* dsp):decorator_dsp(dsp)
        {
            for (int chan = 0; chan < fDSP->getNumInputs(); chan++) {
                fInputLowPass.push_back(FILTER());
            }
            for (int chan = 0; chan < fDSP->getNumOutputs(); chan++) {
                fOutputLowPass.push_back(FILTER());
            }
        }
};

// Down sample-rate adapter
template <typename FILTER>
class dsp_down_sampler : public sr_sampler<FILTER> {
    
    public:
    
        dsp_down_sampler(dsp* dsp):sr_sampler<FILTER>(dsp)
        {}
    
        virtual void init(int sample_rate)
        {
            this->fDSP->init(sample_rate / this->getFactor());
        }
    
        virtual void instanceInit(int sample_rate)
        {
            this->fDSP->instanceInit(sample_rate / this->getFactor());
        }
    
        virtual void instanceConstants(int sample_rate)
        {
            this->fDSP->instanceConstants(sample_rate / this->getFactor());
        }
    
        virtual dsp_down_sampler* clone() { return new dsp_down_sampler(decorator_dsp::clone()); }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            int real_count = count / this->getFactor();
            
            // Adapt inputs
            FAUSTFLOAT** fInputs = (FAUSTFLOAT**)alloca(this->fDSP->getNumInputs() * sizeof(FAUSTFLOAT*));
            for (int chan = 0; chan < this->fDSP->getNumInputs(); chan++) {
                // Lowpass filtering in place on 'inputs'
                this->fInputLowPass[chan].compute(count, inputs[chan], inputs[chan]);
                // Allocate fInputs with 'real_count' frames
                fInputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
                // Decimate
                for (int frame = 0; frame < real_count; frame++) {
                    fInputs[chan][frame] = inputs[chan][frame * this->getFactor()];
                }
            }
            
            // Allocate fOutputs with 'real_count' frames
            FAUSTFLOAT** fOutputs = (FAUSTFLOAT**)alloca(this->fDSP->getNumOutputs() * sizeof(FAUSTFLOAT*));
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                fOutputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
            }
            
            // Compute at lower rate
            this->fDSP->compute(real_count, fInputs, fOutputs);
            
            // Adapt outputs
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                // Puts zeros
                memset(outputs[chan], 0, sizeof(FAUSTFLOAT) * count);
                for (int frame = 0; frame < real_count; frame++) {
                    // Copy one sample every 'DownFactor'
                    // Apply volume
                    //outputs[chan][frame * this->getFactor()] = fOutputs[chan][frame] * this->getFactor();
                    outputs[chan][frame * this->getFactor()] = fOutputs[chan][frame];
                }
                // Lowpass filtering in place on 'outputs'
                this->fOutputLowPass[chan].compute(count, outputs[chan], outputs[chan]);
            }
        }
    
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

// Up sample-rate adapter
template <typename FILTER>
class dsp_up_sampler : public sr_sampler<FILTER> {
    
    public:
    
        dsp_up_sampler(dsp* dsp):sr_sampler<FILTER>(dsp)
        {}
    
        virtual void init(int sample_rate)
        {
            this->fDSP->init(sample_rate * this->getFactor());
        }
    
        virtual void instanceInit(int sample_rate)
        {
            this->fDSP->instanceInit(sample_rate * this->getFactor());
        }
    
        virtual void instanceConstants(int sample_rate)
        {
            this->fDSP->instanceConstants(sample_rate * this->getFactor());
        }
    
        virtual dsp_up_sampler* clone() { return new dsp_up_sampler(decorator_dsp::clone()); }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            int real_count = count * this->getFactor();
            
            // Adapt inputs
            FAUSTFLOAT** fInputs = (FAUSTFLOAT**)alloca(this->fDSP->getNumInputs() * sizeof(FAUSTFLOAT*));
            
            for (int chan = 0; chan < this->fDSP->getNumInputs(); chan++) {
                // Allocate fInputs with 'real_count' frames
                fInputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
                // Puts zeros
                memset(fInputs[chan], 0, sizeof(FAUSTFLOAT) * real_count);
                for (int frame = 0; frame < count; frame++) {
                    // Copy one sample every 'UpFactor'
                    fInputs[chan][frame * this->getFactor()] = inputs[chan][frame];
                }
                // Lowpass filtering in place on 'fInputs'
                this->fInputLowPass[chan].compute(real_count, fInputs[chan], fInputs[chan]);
            }
            
            // Allocate fOutputs with 'real_count' frames
            FAUSTFLOAT** fOutputs = (FAUSTFLOAT**)alloca(this->fDSP->getNumOutputs() * sizeof(FAUSTFLOAT*));
            
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                fOutputs[chan] = (FAUSTFLOAT*)alloca(sizeof(FAUSTFLOAT) * real_count);
            }
            
            // Compute at upper rate
            this->fDSP->compute(real_count, fInputs, fOutputs);
            
            // Adapt outputs
            for (int chan = 0; chan < this->fDSP->getNumOutputs(); chan++) {
                // Lowpass filtering in place on 'fOutputs'
                this->fOutputLowPass[chan].compute(real_count, fOutputs[chan], fOutputs[chan]);
                // Decimate
                for (int frame = 0; frame < count; frame++) {
                    // Apply volume
                    //outputs[chan][frame] = fOutputs[chan][frame * this->getFactor()] * this->getFactor();
                    outputs[chan][frame] = fOutputs[chan][frame * this->getFactor()];
                }
            }
        }
    
        virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
};

// Create a UP/DS + Filter adapted DSP
template <typename REAL>
dsp* createSRAdapter(::dsp* DSP, std::string& error, int ds = 0, int us = 0, int filter = 0)
{
    if (ds >= 2) {
        switch (filter) {
            case 0:
                if (ds == 2) {
                    return new dsp_down_sampler<Identity<Double<1,1>, 2>>(DSP);
                } else if (ds == 3) {
                    return new dsp_down_sampler<Identity<Double<1,1>, 3>>(DSP);
                } else if (ds == 4) {
                    return new dsp_down_sampler<Identity<Double<1,1>, 4>>(DSP);
                } else if (ds == 8) {
                    return new dsp_down_sampler<Identity<Double<1,1>, 8>>(DSP);
                } else if (ds == 16) {
                    return new dsp_down_sampler<Identity<Double<1,1>, 16>>(DSP);
                } else if (ds == 32) {
                    return new dsp_down_sampler<Identity<Double<1,1>, 32>>(DSP);
                } else {
                    error = "ERROR : ds factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 1:
                if (ds == 2) {
                    return new dsp_down_sampler<LowPass3<Double<45,100>, 2, REAL>>(DSP);
                } else if (ds == 3) {
                    return new dsp_down_sampler<LowPass3<Double<45,100>, 3, REAL>>(DSP);
                } else if (ds == 4) {
                    return new dsp_down_sampler<LowPass3<Double<45,100>, 4, REAL>>(DSP);
                } else if (ds == 8) {
                    return new dsp_down_sampler<LowPass3<Double<45,100>, 8, REAL>>(DSP);
                } else if (ds == 16) {
                    return new dsp_down_sampler<LowPass3<Double<45,100>, 16, REAL>>(DSP);
                } else if (ds == 32) {
                    return new dsp_down_sampler<LowPass3<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : ds factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 2:
                if (ds == 2) {
                    return new dsp_down_sampler<LowPass4<Double<45,100>, 2, REAL>>(DSP);
                } else if (ds == 3) {
                    return new dsp_down_sampler<LowPass4<Double<45,100>, 3, REAL>>(DSP);
                } else if (ds == 4) {
                    return new dsp_down_sampler<LowPass4<Double<45,100>, 4, REAL>>(DSP);
                } else if (ds == 8) {
                    return new dsp_down_sampler<LowPass4<Double<45,100>, 8, REAL>>(DSP);
                } else if (ds == 16) {
                    return new dsp_down_sampler<LowPass4<Double<45,100>, 16, REAL>>(DSP);
                } else if (ds == 32) {
                    return new dsp_down_sampler<LowPass4<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : ds factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 3:
                if (ds == 2) {
                    return new dsp_down_sampler<LowPass3e<Double<45,100>, 2, REAL>>(DSP);
                } else if (ds == 3) {
                    return new dsp_down_sampler<LowPass3e<Double<45,100>, 3, REAL>>(DSP);
                } else if (ds == 4) {
                    return new dsp_down_sampler<LowPass3e<Double<45,100>, 4, REAL>>(DSP);
                } else if (ds == 8) {
                    return new dsp_down_sampler<LowPass3e<Double<45,100>, 8, REAL>>(DSP);
                } else if (ds == 16) {
                    return new dsp_down_sampler<LowPass3e<Double<45,100>, 16, REAL>>(DSP);
                } else if (ds == 32) {
                    return new dsp_down_sampler<LowPass3e<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : ds factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 4:
                if (ds == 2) {
                    return new dsp_down_sampler<LowPass6e<Double<45,100>, 2, REAL>>(DSP);
                } else if (ds == 3) {
                    return new dsp_down_sampler<LowPass6e<Double<45,100>, 3, REAL>>(DSP);
                } else if (ds == 4) {
                    return new dsp_down_sampler<LowPass6e<Double<45,100>, 4, REAL>>(DSP);
                } else if (ds == 8) {
                    return new dsp_down_sampler<LowPass6e<Double<45,100>, 8, REAL>>(DSP);
                } else if (ds == 16) {
                    return new dsp_down_sampler<LowPass6e<Double<45,100>, 16, REAL>>(DSP);
                } else if (ds == 32) {
                    return new dsp_down_sampler<LowPass6e<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : ds factor type must be in [2..32] range\n";
                    return nullptr;
                }
            default:
                error = "ERROR : filter type must be in [0..4] range\n";
                return nullptr;
        }
    } else if (us >= 2) {
        
        switch (filter) {
            case 0:
                if (us == 2) {
                    return new dsp_up_sampler<Identity<Double<1,1>, 2>>(DSP);
                } else if (us == 3) {
                    return new dsp_up_sampler<Identity<Double<1,1>, 3>>(DSP);
                } else if (us == 4) {
                    return new dsp_up_sampler<Identity<Double<1,1>, 4>>(DSP);
                } else if (us == 8) {
                    return new dsp_up_sampler<Identity<Double<1,1>, 8>>(DSP);
                } else if (us == 16) {
                    return new dsp_up_sampler<Identity<Double<1,1>, 16>>(DSP);
                } else if (us == 32) {
                    return new dsp_up_sampler<Identity<Double<1,1>, 32>>(DSP);
                } else {
                    error = "ERROR : us factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 1:
                if (us == 2) {
                    return new dsp_up_sampler<LowPass3<Double<45,100>, 2, REAL>>(DSP);
                } else if (us == 3) {
                    return new dsp_up_sampler<LowPass3<Double<45,100>, 3, REAL>>(DSP);
                } else if (us == 4) {
                    return new dsp_up_sampler<LowPass3<Double<45,100>, 4, REAL>>(DSP);
                } else if (us == 8) {
                    return new dsp_up_sampler<LowPass3<Double<45,100>, 8, REAL>>(DSP);
                } else if (us == 16) {
                    return new dsp_up_sampler<LowPass3<Double<45,100>, 16, REAL>>(DSP);
                } else if (us == 32) {
                    return new dsp_up_sampler<LowPass3<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : us factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 2:
                if (us == 2) {
                    return new dsp_up_sampler<LowPass4<Double<45,100>, 2, REAL>>(DSP);
                } else if (us == 3) {
                    return new dsp_up_sampler<LowPass4<Double<45,100>, 3, REAL>>(DSP);
                } else if (us == 4) {
                    return new dsp_up_sampler<LowPass4<Double<45,100>, 4, REAL>>(DSP);
                } else if (us == 8) {
                    return new dsp_up_sampler<LowPass4<Double<45,100>, 8, REAL>>(DSP);
                } else if (us == 16) {
                    return new dsp_up_sampler<LowPass4<Double<45,100>, 16, REAL>>(DSP);
                } else if (us == 32) {
                    return new dsp_up_sampler<LowPass4<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : us factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 3:
                if (us == 2) {
                    return new dsp_up_sampler<LowPass3e<Double<45,100>, 2, REAL>>(DSP);
                } else if (us == 3) {
                    return new dsp_up_sampler<LowPass3e<Double<45,100>, 3, REAL>>(DSP);
                } else if (us == 4) {
                    return new dsp_up_sampler<LowPass3e<Double<45,100>, 4, REAL>>(DSP);
                } else if (us == 8) {
                    return new dsp_up_sampler<LowPass3e<Double<45,100>, 8, REAL>>(DSP);
                } else if (us == 16) {
                    return new dsp_up_sampler<LowPass3e<Double<45,100>, 16, REAL>>(DSP);
                } else if (us == 32) {
                    return new dsp_up_sampler<LowPass3e<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : us factor type must be in [2..32] range\n";
                    return nullptr;
                }
            case 4:
                if (us == 2) {
                    return new dsp_up_sampler<LowPass6e<Double<45,100>, 2, REAL>>(DSP);
                } else if (us == 3) {
                    return new dsp_up_sampler<LowPass6e<Double<45,100>, 3, REAL>>(DSP);
                } else if (us == 4) {
                    return new dsp_up_sampler<LowPass6e<Double<45,100>, 4, REAL>>(DSP);
                } else if (us == 8) {
                    return new dsp_up_sampler<LowPass6e<Double<45,100>, 8, REAL>>(DSP);
                } else if (us == 16) {
                    return new dsp_up_sampler<LowPass6e<Double<45,100>, 16, REAL>>(DSP);
                } else if (us == 32) {
                    return new dsp_up_sampler<LowPass6e<Double<45,100>, 32, REAL>>(DSP);
                } else {
                    error = "ERROR : us factor type must be in [2..32] range\n";
                    return nullptr;
                }
            default:
                error = "ERROR : filter type must be in [0..4] range\n";
                return nullptr;
        }
    } else {
        return DSP;
    }
}
    
#endif
/************************** END dsp-adapter.h **************************/
/************************** BEGIN proxy-dsp.h ***************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
***************************************************************************/

#ifndef __proxy_dsp__
#define __proxy_dsp__

#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <cassert>

/************************** BEGIN DecoratorUI.h **************************
 FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
*************************************************************************/

#ifndef Decorator_UI_H
#define Decorator_UI_H


//----------------------------------------------------------------
//  Generic UI empty implementation
//----------------------------------------------------------------

class FAUST_API GenericUI : public UI
{
    
    public:
        
        GenericUI() {}
        virtual ~GenericUI() {}

#ifdef DAISY_NO_RTTI
        virtual bool isSoundUI() const override { return true; }
#endif
        
        // -- widget's layouts
        virtual void openTabBox(const char* label) {}
        virtual void openHorizontalBox(const char* label) {}
        virtual void openVerticalBox(const char* label) {}
        virtual void closeBox() {}
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone) {}
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}
    
        // -- passive widgets
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max) {}
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* soundpath, Soundfile** sf_zone) {}
    
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) {}
    
};

//----------------------------------------------------------------
//  Generic UI decorator
//----------------------------------------------------------------

class FAUST_API DecoratorUI : public UI
{
    
    protected:
        
        UI* fUI;
        
    public:
        
        DecoratorUI(UI* ui = 0):fUI(ui) {}
        virtual ~DecoratorUI() { delete fUI; }
        
        // -- widget's layouts
        virtual void openTabBox(const char* label)          { fUI->openTabBox(label); }
        virtual void openHorizontalBox(const char* label)   { fUI->openHorizontalBox(label); }
        virtual void openVerticalBox(const char* label)     { fUI->openVerticalBox(label); }
        virtual void closeBox()                             { fUI->closeBox(); }
        
        // -- active widgets
        virtual void addButton(const char* label, FAUSTFLOAT* zone)         { fUI->addButton(label, zone); }
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone)    { fUI->addCheckButton(label, zone); }
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addVerticalSlider(label, zone, init, min, max, step); }
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addHorizontalSlider(label, zone, init, min, max, step); }
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step)
        { fUI->addNumEntry(label, zone, init, min, max, step); }
        
        // -- passive widgets
        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        { fUI->addHorizontalBargraph(label, zone, min, max); }
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max)
        { fUI->addVerticalBargraph(label, zone, min, max); }
    
        // -- soundfiles
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) { fUI->addSoundfile(label, filename, sf_zone); }
    
        virtual void declare(FAUSTFLOAT* zone, const char* key, const char* val) { fUI->declare(zone, key, val); }
    
};

// Defined here to simplify header #include inclusion 
class FAUST_API SoundUIInterface : public GenericUI {};

#endif
/**************************  END  DecoratorUI.h **************************/
/************************** BEGIN JSONUIDecoder.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 *************************************************************************/

#ifndef __JSONUIDecoder__
#define __JSONUIDecoder__

#include <vector>
#include <map>
#include <utility>
#include <cstdlib>
#include <functional>
#include <sstream>
#include <cstring>
#include <string_view>

/************************** BEGIN CGlue.h *****************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
*************************************************************************/

#ifndef CGLUE_H
#define CGLUE_H

/************************** BEGIN CInterface.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 *************************************************************************/

#ifndef CINTERFACE_H
#define CINTERFACE_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
    
struct Soundfile;

/*******************************************************************************
 * UI, Meta and MemoryManager structures for C code.
 ******************************************************************************/

// -- widget's layouts

typedef void (* openTabBoxFun) (void* ui_interface, const char* label);
typedef void (* openHorizontalBoxFun) (void* ui_interface, const char* label);
typedef void (* openVerticalBoxFun) (void* ui_interface, const char* label);
typedef void (* closeBoxFun) (void* ui_interface);

// -- active widgets

typedef void (* addButtonFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone);
typedef void (* addCheckButtonFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone);
typedef void (* addVerticalSliderFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
typedef void (* addHorizontalSliderFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
typedef void (* addNumEntryFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

// -- passive widgets

typedef void (* addHorizontalBargraphFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);
typedef void (* addVerticalBargraphFun) (void* ui_interface, const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

// -- soundfiles
    
typedef void (* addSoundfileFun) (void* ui_interface, const char* label, const char* url, struct Soundfile** sf_zone);

typedef void (* declareFun) (void* ui_interface, FAUSTFLOAT* zone, const char* key, const char* value);

typedef struct {

    void* uiInterface;

    openTabBoxFun openTabBox;
    openHorizontalBoxFun openHorizontalBox;
    openVerticalBoxFun openVerticalBox;
    closeBoxFun closeBox;
    addButtonFun addButton;
    addCheckButtonFun addCheckButton;
    addVerticalSliderFun addVerticalSlider;
    addHorizontalSliderFun addHorizontalSlider;
    addNumEntryFun addNumEntry;
    addHorizontalBargraphFun addHorizontalBargraph;
    addVerticalBargraphFun addVerticalBargraph;
    addSoundfileFun addSoundfile;
    declareFun declare;

} UIGlue;

typedef void (* metaDeclareFun) (void* ui_interface, const char* key, const char* value);

typedef struct {

    void* metaInterface;
    
    metaDeclareFun declare;

} MetaGlue;

/***************************************
 *  Interface for the DSP object
 ***************************************/

typedef char dsp_imp;
    
typedef dsp_imp* (* newDspFun) ();
typedef void (* destroyDspFun) (dsp_imp* dsp);
typedef int (* getNumInputsFun) (dsp_imp* dsp);
typedef int (* getNumOutputsFun) (dsp_imp* dsp);
typedef void (* buildUserInterfaceFun) (dsp_imp* dsp, UIGlue* ui);
typedef int (* getSampleRateFun) (dsp_imp* dsp);
typedef void (* initFun) (dsp_imp* dsp, int sample_rate);
typedef void (* classInitFun) (int sample_rate);
typedef void (* staticInitFun) (dsp_imp* dsp, int sample_rate);
typedef void (* instanceInitFun) (dsp_imp* dsp, int sample_rate);
typedef void (* instanceConstantsFun) (dsp_imp* dsp, int sample_rate);
typedef void (* instanceResetUserInterfaceFun) (dsp_imp* dsp);
typedef void (* instanceClearFun) (dsp_imp* dsp);
typedef void (* computeFun) (dsp_imp* dsp, int len, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs);
typedef void (* metadataFun) (MetaGlue* meta);
    
/***************************************
 * DSP memory manager functions
 ***************************************/

typedef void* (* allocateFun) (void* manager_interface, size_t size);
typedef void (* destroyFun) (void* manager_interface, void* ptr);

typedef struct {
    
    void* managerInterface;
    
    allocateFun allocate;
    destroyFun destroy;
    
} MemoryManagerGlue;

#ifdef __cplusplus
}
#endif

#endif
/**************************  END  CInterface.h **************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * UI glue code
 ******************************************************************************/
 
class UIFloat
{

    public:

        UIFloat() {}

        virtual ~UIFloat() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, float* zone) = 0;
        virtual void addCheckButton(const char* label, float* zone) = 0;
        virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
        virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
        virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max) = 0;
        virtual void addVerticalBargraph(const char* label, float* zone, float min, float max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(float* zone, const char* key, const char* val) {}
};

static void openTabBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openTabBox(label);
}

static void openHorizontalBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openHorizontalBox(label);
}

static void openVerticalBoxGlueFloat(void* cpp_interface, const char* label)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->openVerticalBox(label);
}

static void closeBoxGlueFloat(void* cpp_interface)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->closeBox();
}

static void addButtonGlueFloat(void* cpp_interface, const char* label, float* zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addButton(label, zone);
}

static void addCheckButtonGlueFloat(void* cpp_interface, const char* label, float* zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addCheckButton(label, zone);
}

static void addVerticalSliderGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addVerticalSlider(label, zone, init, min, max, step);
}

static void addHorizontalSliderGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addHorizontalSlider(label, zone, init, min, max, step);
}

static void addNumEntryGlueFloat(void* cpp_interface, const char* label, float* zone, float init, float min, float max, float step)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addNumEntry(label, zone, init, min, max, step);
}

static void addHorizontalBargraphGlueFloat(void* cpp_interface, const char* label, float* zone, float min, float max)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addHorizontalBargraph(label, zone, min, max);
}

static void addVerticalBargraphGlueFloat(void* cpp_interface, const char* label, float* zone, float min, float max)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addVerticalBargraph(label, zone, min, max);
}
    
static void addSoundfileGlueFloat(void* cpp_interface, const char* label, const char* url, Soundfile** sf_zone)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->addSoundfile(label, url, sf_zone);
}

static void declareGlueFloat(void* cpp_interface, float* zone, const char* key, const char* value)
{
    UIFloat* ui_interface = static_cast<UIFloat*>(cpp_interface);
    ui_interface->declare(zone, key, value);
}

class UIDouble
{

    public:

        UIDouble() {}

        virtual ~UIDouble() {}

        // -- widget's layouts

        virtual void openTabBox(const char* label) = 0;
        virtual void openHorizontalBox(const char* label) = 0;
        virtual void openVerticalBox(const char* label) = 0;
        virtual void closeBox() = 0;

        // -- active widgets

        virtual void addButton(const char* label, double* zone) = 0;
        virtual void addCheckButton(const char* label, double* zone) = 0;
        virtual void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
        virtual void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
        virtual void addNumEntry(const char* label, double* zone, double init, double min, double max, double step) = 0;

        // -- passive widgets

        virtual void addHorizontalBargraph(const char* label, double* zone, double min, double max) = 0;
        virtual void addVerticalBargraph(const char* label, double* zone, double min, double max) = 0;
    
        // -- soundfiles
    
        virtual void addSoundfile(const char* label, const char* filename, Soundfile** sf_zone) = 0;

        // -- metadata declarations

        virtual void declare(double* zone, const char* key, const char* val) {}
};

static void openTabBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openTabBox(label);
}

static void openHorizontalBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openHorizontalBox(label);
}

static void openVerticalBoxGlueDouble(void* cpp_interface, const char* label)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->openVerticalBox(label);
}

static void closeBoxGlueDouble(void* cpp_interface)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->closeBox();
}

static void addButtonGlueDouble(void* cpp_interface, const char* label, double* zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addButton(label, zone);
}

static void addCheckButtonGlueDouble(void* cpp_interface, const char* label, double* zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addCheckButton(label, zone);
}

static void addVerticalSliderGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addVerticalSlider(label, zone, init, min, max, step);
}

static void addHorizontalSliderGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addHorizontalSlider(label, zone, init, min, max, step);
}

static void addNumEntryGlueDouble(void* cpp_interface, const char* label, double* zone, double init, double min, double max, double step)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addNumEntry(label, zone, init, min, max, step);
}

static void addHorizontalBargraphGlueDouble(void* cpp_interface, const char* label, double* zone, double min, double max)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addHorizontalBargraph(label, zone, min, max);
}

static void addVerticalBargraphGlueDouble(void* cpp_interface, const char* label, double* zone, double min, double max)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addVerticalBargraph(label, zone, min, max);
}
    
static void addSoundfileGlueDouble(void* cpp_interface, const char* label, const char* url, Soundfile** sf_zone)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->addSoundfile(label, url, sf_zone);
}

static void declareGlueDouble(void* cpp_interface, double* zone, const char* key, const char* value)
{
    UIDouble* ui_interface = static_cast<UIDouble*>(cpp_interface);
    ui_interface->declare(zone, key, value);
}

static void buildUIGlue(UIGlue* glue, UI* ui_interface, bool is_double)
{
    glue->uiInterface = ui_interface;
    
    if (is_double) {
        glue->openTabBox = reinterpret_cast<openTabBoxFun>(openTabBoxGlueDouble);
        glue->openHorizontalBox = reinterpret_cast<openHorizontalBoxFun>(openHorizontalBoxGlueDouble);
        glue->openVerticalBox = reinterpret_cast<openVerticalBoxFun>(openVerticalBoxGlueDouble);
        glue->closeBox = reinterpret_cast<closeBoxFun>(closeBoxGlueDouble);
        glue->addButton = reinterpret_cast<addButtonFun>(addButtonGlueDouble);
        glue->addCheckButton = reinterpret_cast<addCheckButtonFun>(addCheckButtonGlueDouble);
        glue->addVerticalSlider = reinterpret_cast<addVerticalSliderFun>(addVerticalSliderGlueDouble);
        glue->addHorizontalSlider = reinterpret_cast<addHorizontalSliderFun>(addHorizontalSliderGlueDouble);
        glue->addNumEntry = reinterpret_cast<addNumEntryFun>(addNumEntryGlueDouble);
        glue->addHorizontalBargraph = reinterpret_cast<addHorizontalBargraphFun>(addHorizontalBargraphGlueDouble);
        glue->addVerticalBargraph = reinterpret_cast<addVerticalBargraphFun>(addVerticalBargraphGlueDouble);
        glue->addSoundfile = reinterpret_cast<addSoundfileFun>(addSoundfileGlueDouble);
        glue->declare = reinterpret_cast<declareFun>(declareGlueDouble);
    } else {
        glue->openTabBox = reinterpret_cast<openTabBoxFun>(openTabBoxGlueFloat);
        glue->openHorizontalBox = reinterpret_cast<openHorizontalBoxFun>(openHorizontalBoxGlueFloat);
        glue->openVerticalBox = reinterpret_cast<openVerticalBoxFun>(openVerticalBoxGlueFloat);
        glue->closeBox = reinterpret_cast<closeBoxFun>(closeBoxGlueFloat);
        glue->addButton = reinterpret_cast<addButtonFun>(addButtonGlueFloat);
        glue->addCheckButton = reinterpret_cast<addCheckButtonFun>(addCheckButtonGlueFloat);
        glue->addVerticalSlider = reinterpret_cast<addVerticalSliderFun>(addVerticalSliderGlueFloat);
        glue->addHorizontalSlider = reinterpret_cast<addHorizontalSliderFun>(addHorizontalSliderGlueFloat);
        glue->addNumEntry = reinterpret_cast<addNumEntryFun>(addNumEntryGlueFloat);
        glue->addHorizontalBargraph = reinterpret_cast<addHorizontalBargraphFun>(addHorizontalBargraphGlueFloat);
        glue->addVerticalBargraph = reinterpret_cast<addVerticalBargraphFun>(addVerticalBargraphGlueFloat);
        glue->addSoundfile = reinterpret_cast<addSoundfileFun>(addSoundfileGlueFloat);
        glue->declare = reinterpret_cast<declareFun>(declareGlueFloat);
    }
}
    
// Base class
    
struct UIInterface
{
    virtual ~UIInterface() {}
    
    virtual int sizeOfFAUSTFLOAT() = 0;
    
    // -- widget's layouts
    
    virtual void openTabBox(const char* label) = 0;
    virtual void openHorizontalBox(const char* label) = 0;
    virtual void openVerticalBox(const char* label) = 0;
    virtual void closeBox() = 0;
    
    // float version
    
    // -- active widgets
    
    virtual void addButton(const char* label, float* zone) = 0;
    virtual void addCheckButton(const char* label, float* zone) = 0;
    
    virtual void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
    virtual void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step) = 0;
    virtual void addNumEntry(const char* label, float* zone, float init, float min, float max, float step) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, float* zone, float min, float max) = 0;
    virtual void addVerticalBargraph(const char* label, float* zone, float min, float max) = 0;
    
    // -- metadata declarations
    
    virtual void declare(float* zone, const char* key, const char* val) = 0;
    
    // double version
    
    virtual void addButton(const char* label, double* zone) = 0;
    virtual void addCheckButton(const char* label, double* zone) = 0;
  
    virtual void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
    virtual void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step) = 0;
    
    virtual void addNumEntry(const char* label, double* zone, double init, double min, double max, double step) = 0;
    
    // -- soundfiles
    
    virtual void addSoundfile(const char* label, const char* url, Soundfile** sf_zone) = 0;
    
    // -- passive widgets
    
    virtual void addHorizontalBargraph(const char* label, double* zone, double min, double max) = 0;
    virtual void addVerticalBargraph(const char* label, double* zone, double min, double max) = 0;
     
    // -- metadata declarations
    
    virtual void declare(double* zone, const char* key, const char* val) = 0;
    
};
    
struct UITemplate : public UIInterface
{
 
    void* fCPPInterface;

    UITemplate(void* cpp_interface):fCPPInterface(cpp_interface)
    {}
    virtual ~UITemplate() {}
    
    int sizeOfFAUSTFLOAT()
    {
        return reinterpret_cast<UI*>(fCPPInterface)->sizeOfFAUSTFLOAT();
    }
    
    // -- widget's layouts
    
    void openTabBox(const char* label)
    {
        openTabBoxGlueFloat(fCPPInterface, label);
    }
    void openHorizontalBox(const char* label)
    {
        openHorizontalBoxGlueFloat(fCPPInterface, label);
    }
    void openVerticalBox(const char* label)
    {
        openVerticalBoxGlueFloat(fCPPInterface, label);
    }
    void closeBox()
    {
        closeBoxGlueFloat(fCPPInterface);
    }
    
    // float version
    
    // -- active widgets
    
    void addButton(const char* label, float* zone)
    {
        addButtonGlueFloat(fCPPInterface, label, zone);
    }
    void addCheckButton(const char* label, float* zone)
    {
        addCheckButtonGlueFloat(fCPPInterface, label, zone);
    }
    
    void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step)
    {
        addVerticalSliderGlueFloat(fCPPInterface, label, zone, init, min, max, step);
    }
    
    void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step)
    {
        addHorizontalSliderGlueFloat(fCPPInterface, label, zone, init, min, max, step);
    }
    
    void addNumEntry(const char* label, float* zone, float init, float min, float max, float step)
    {
        addNumEntryGlueFloat(fCPPInterface, label, zone, init, min, max, step);
    }
    
    // -- passive widgets
    
    void addHorizontalBargraph(const char* label, float* zone, float min, float max)
    {
        addHorizontalBargraphGlueFloat(fCPPInterface, label, zone, min, max);
    }
    
    void addVerticalBargraph(const char* label, float* zone, float min, float max)
    {
        addVerticalBargraphGlueFloat(fCPPInterface, label, zone, min, max);
    }

    // -- metadata declarations
    
    void declare(float* zone, const char* key, const char* val)
    {
        declareGlueFloat(fCPPInterface, zone, key, val);
    }
    
    // double version
    
    void addButton(const char* label, double* zone)
    {
        addButtonGlueDouble(fCPPInterface, label, zone);
    }
    void addCheckButton(const char* label, double* zone)
    {
        addCheckButtonGlueDouble(fCPPInterface, label, zone);
    }
    
    void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step)
    {
        addVerticalSliderGlueDouble(fCPPInterface, label, zone, init, min, max, step);
    }
    
    void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step)
    {
        addHorizontalSliderGlueDouble(fCPPInterface, label, zone, init, min, max, step);
    }
    
    void addNumEntry(const char* label, double* zone, double init, double min, double max, double step)
    {
        addNumEntryGlueDouble(fCPPInterface, label, zone, init, min, max, step);
    }

    // -- soundfiles
    
    void addSoundfile(const char* label, const char* url, Soundfile** sf_zone)
    {
        addSoundfileGlueFloat(fCPPInterface, label, url, sf_zone);
    }

    // -- passive widgets
    
    void addHorizontalBargraph(const char* label, double* zone, double min, double max)
    {
        addHorizontalBargraphGlueDouble(fCPPInterface, label, zone, min, max);
    }
    
    void addVerticalBargraph(const char* label, double* zone, double min, double max)
    {
        addVerticalBargraphGlueDouble(fCPPInterface, label, zone, min, max);
    }

    // -- metadata declarations
    
    void declare(double* zone, const char* key, const char* val)
    {
        declareGlueDouble(fCPPInterface, zone, key, val);
    }

};
    
struct UIGlueTemplate : public UIInterface
{
    
    UIGlue* fGlue;
    
    UIGlueTemplate(UIGlue* glue):fGlue(glue)
    {}
    virtual ~UIGlueTemplate() {}
    
    virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
    
    // -- widget's layouts
    
    void openTabBox(const char* label)
    {
        fGlue->openTabBox(fGlue->uiInterface, label);
    }
    void openHorizontalBox(const char* label)
    {
        fGlue->openHorizontalBox(fGlue->uiInterface, label);
    }
    void openVerticalBox(const char* label)
    {
        fGlue->openVerticalBox(fGlue->uiInterface, label);
    }
    void closeBox()
    {
        fGlue->closeBox(fGlue->uiInterface);
    }

    // float version
    
    // -- active widgets
    
    void addButton(const char* label, float* zone)
    {
        fGlue->addButton(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone));
    }
    void addCheckButton(const char* label, float* zone)
    {
        fGlue->addCheckButton(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone));
    }
    
    void addVerticalSlider(const char* label, float* zone, float init, float min, float max, float step)
    {
        fGlue->addVerticalSlider(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), init, min, max, step);
    }
    void addHorizontalSlider(const char* label, float* zone, float init, float min, float max, float step)
    {
        fGlue->addHorizontalSlider(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), init, min, max, step);
    }
    void addNumEntry(const char* label, float* zone, float init, float min, float max, float step)
    {
        fGlue->addNumEntry(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), init, min, max, step);
    }
    
    // -- passive widgets
    
    void addHorizontalBargraph(const char* label, float* zone, float min, float max)
    {
        fGlue->addHorizontalBargraph(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), min, max);
    }
    void addVerticalBargraph(const char* label, float* zone, float min, float max)
    {
        fGlue->addVerticalBargraph(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), min, max);
    }
    
    // -- metadata declarations
    
    void declare(float* zone, const char* key, const char* val)
    {
        fGlue->declare(fGlue->uiInterface, reinterpret_cast<FAUSTFLOAT*>(zone), key, val);
    }
    
    // double version
    
    void addButton(const char* label, double* zone)
    {
        fGlue->addButton(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone));
    }
    void addCheckButton(const char* label, double* zone)
    {
        fGlue->addCheckButton(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone));
    }
    
    void addVerticalSlider(const char* label, double* zone, double init, double min, double max, double step)
    {
        fGlue->addVerticalSlider(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), init, min, max, step);
    }
    void addHorizontalSlider(const char* label, double* zone, double init, double min, double max, double step)
    {
        fGlue->addHorizontalSlider(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), init, min, max, step);
    }
    void addNumEntry(const char* label, double* zone, double init, double min, double max, double step)
    {
        fGlue->addNumEntry(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), init, min, max, step);
    }
    // -- soundfiles
    
    void addSoundfile(const char* label, const char* url, Soundfile** sf_zone) {}
    
    // -- passive widgets
    
    void addHorizontalBargraph(const char* label, double* zone, double min, double max)
    {
        fGlue->addHorizontalBargraph(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), min, max);
    }
    void addVerticalBargraph(const char* label, double* zone, double min, double max)
    {
        fGlue->addVerticalBargraph(fGlue->uiInterface, label, reinterpret_cast<FAUSTFLOAT*>(zone), min, max);
    }
    
    // -- metadata declarations
    
    void declare(double* zone, const char* key, const char* val)
    {
        fGlue->declare(fGlue->uiInterface, reinterpret_cast<FAUSTFLOAT*>(zone), key, val);
    }
    
};

/*******************************************************************************
 * Meta glue code
 ******************************************************************************/

static void declareMetaGlue(void* cpp_interface, const char* key, const char* value)
{
    Meta* meta_interface = static_cast<Meta*>(cpp_interface);
    meta_interface->declare(key, value);
}

static void buildMetaGlue(MetaGlue* glue, Meta* meta)
{
    glue->metaInterface = meta;
    glue->declare = declareMetaGlue;
}
    
/*******************************************************************************
 * Memory manager glue code
 ******************************************************************************/

static void* allocateMemoryManagerGlue(void* cpp_interface, size_t size)
{
    dsp_memory_manager* manager_interface = static_cast<dsp_memory_manager*>(cpp_interface);
    return manager_interface->allocate(size);
}
    
static void destroyMemoryManagerGlue(void* cpp_interface, void* ptr)
{
    dsp_memory_manager* manager_interface = static_cast<dsp_memory_manager*>(cpp_interface);
    manager_interface->destroy(ptr);
}

static void buildManagerGlue(MemoryManagerGlue* glue, dsp_memory_manager* manager)
{
    glue->managerInterface = manager;
    glue->allocate = allocateMemoryManagerGlue;
    glue->destroy = destroyMemoryManagerGlue;
}

#ifdef __cplusplus
}
#endif

#endif
/**************************  END  CGlue.h **************************/

#ifdef _WIN32
#include <windows.h>
#define snprintf _snprintf
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif

//------------------------------------------------------------------------------------------
//  Decode a dsp JSON description and implement 'buildUserInterface' and 'metadata' methods
//------------------------------------------------------------------------------------------

#define REAL_UI(ui_interface) reinterpret_cast<UIReal<REAL>*>(ui_interface)
#define REAL_ADR(index)      reinterpret_cast<REAL*>(&memory_block[index])
#define REAL_EXT_ADR(index)  reinterpret_cast<FAUSTFLOAT*>(&memory_block[index])
#define SOUNDFILE_ADR(index) reinterpret_cast<Soundfile**>(&memory_block[index])

typedef std::function<void(FAUSTFLOAT)> ReflectFunction;
typedef std::function<FAUSTFLOAT()> ModifyFunction;

struct FAUST_API ExtZoneParam {

    virtual void reflectZone() = 0;
    virtual void modifyZone() = 0;
    
    virtual void setReflectZoneFun(ReflectFunction reflect) = 0;
    virtual void setModifyZoneFun(ModifyFunction modify) = 0;
    
    virtual ~ExtZoneParam()
    {}
    
};

// Templated decoder

struct FAUST_API JSONUIDecoderBase
{
    virtual ~JSONUIDecoderBase()
    {}
    
    virtual void metadata(Meta* m) = 0;
    virtual void metadata(MetaGlue* glue) = 0;
    virtual int getDSPSize() = 0;
    virtual std::string getName() = 0;
    virtual std::string getLibVersion() = 0;
    virtual std::string getCompileOptions() = 0;
    virtual std::vector<std::string> getLibraryList() = 0;
    virtual std::vector<std::string> getIncludePathnames() = 0;
    virtual int getNumInputs() = 0;
    virtual int getNumOutputs() = 0;
    virtual int getSampleRate(char* memory_block) = 0;
    virtual void setReflectZoneFun(int index, ReflectFunction fun) = 0;
    virtual void setModifyZoneFun(int index, ModifyFunction fun) = 0;
    virtual void setupDSPProxy(UI* ui_interface, char* memory_block) = 0;
    virtual bool hasDSPProxy() = 0;
    virtual std::vector<ExtZoneParam*>& getInputControls() = 0;
    virtual std::vector<ExtZoneParam*>& getOutputControls() = 0;
    virtual void resetUserInterface() = 0;
    virtual void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr) = 0;
    virtual void buildUserInterface(UI* ui_interface) = 0;
    virtual void buildUserInterface(UI* ui_interface, char* memory_block) = 0;
    virtual void buildUserInterface(UIGlue* ui_interface, char* memory_block) = 0;
    virtual bool hasCompileOption(const std::string& option) = 0;
    virtual std::string getCompileOption(const std::string& option) = 0;
};

template <typename REAL>
struct FAUST_API JSONUIDecoderReal : public JSONUIDecoderBase {
    
    struct ZoneParam : public ExtZoneParam {
        
        FAUSTFLOAT fZone;
        ReflectFunction fReflect;
        ModifyFunction fModify;
        
    #if defined(TARGET_OS_IPHONE) || defined(WIN32)
        ZoneParam(ReflectFunction reflect = nullptr, ModifyFunction modify = nullptr)
        :fReflect(reflect), fModify(modify)
        {}
        void reflectZone() { if (fReflect) fReflect(fZone); }
        void modifyZone() { if (fModify) fZone = fModify(); }
    #else
        ZoneParam(ReflectFunction reflect = [](FAUSTFLOAT value) {}, ModifyFunction modify = []() { return FAUSTFLOAT(-1); })
        :fReflect(reflect), fModify(modify)
        {}
        void reflectZone() { fReflect(fZone); }
        void modifyZone() { fZone = fModify(); }
    #endif
        
        void setReflectZoneFun(ReflectFunction reflect) { fReflect = reflect; }
        void setModifyZoneFun(ModifyFunction modify) { fModify = modify; }
        
    };
    
    typedef std::vector<ExtZoneParam*> controlMap;
  
    std::string fName;
    std::string fFileName;
    std::string fJSON;
    std::string fVersion;
    std::string fCompileOptions;
    
    std::map<std::string, std::string> fMetadata;
    std::vector<itemInfo> fUiItems;
    
    std::vector<std::string> fLibraryList;
    std::vector<std::string> fIncludePathnames;
    
    int fNumInputs, fNumOutputs, fSRIndex;
    int fDSPSize;
    bool fDSPProxy;
    
    controlMap fPathInputTable;     // [path, ZoneParam]
    controlMap fPathOutputTable;    // [path, ZoneParam]
    
    bool startWith(const std::string& str, const std::string& prefix) const noexcept
    {
        return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
    }

    bool isInput(const std::string& type) const noexcept
    {
        return (type == "vslider" || type == "hslider" || type == "nentry" || type == "button" || type == "checkbox");
    }
    bool isOutput(const std::string& type) const noexcept { return (type == "hbargraph" || type == "vbargraph"); }
    bool isSoundfile(const std::string& type) const noexcept { return (type == "soundfile"); }
    
    std::string getString(std::map<std::string, std::pair<std::string, double> >& map, const std::string& key)
    {
        return (map.find(key) != map.end()) ? map[key].first : "";
    }
    
    int getInt(std::map<std::string, std::pair<std::string, double> >& map, const std::string& key)
    {
        return (map.find(key) != map.end()) ? int(map[key].second) : -1;
    }
    
    void setReflectZoneFun(int index, ReflectFunction fun) override
    {
        fPathInputTable[index]->setReflectZoneFun(fun);
    }
    
    void setModifyZoneFun(int index, ModifyFunction fun) override
    {
        fPathOutputTable[index]->setModifyZoneFun(fun);
    }

    JSONUIDecoderReal(const std::string& json)
    {
        initFromJSON(json);
    }

    void initFromJSON(const std::string& json)
    {
        fJSON = json;
        const char* p = fJSON.c_str();
        std::map<std::string, std::pair<std::string, double> > meta_data1;
        std::map<std::string, std::vector<std::string> > meta_data2;
        parseJson(p, meta_data1, fMetadata, meta_data2, fUiItems);
        
        // meta_data1 contains <name : val>, <inputs : val>, <ouputs : val> pairs etc...
        fName = getString(meta_data1, "name");
        fFileName = getString(meta_data1, "filename");
        fVersion = getString(meta_data1, "version");
        fCompileOptions = getString(meta_data1, "compile_options");
        
        if (meta_data2.find("library_list") != meta_data2.end()) {
            fLibraryList = meta_data2["library_list"];
        } else {
            // 'library_list' is coded as successive 'library_pathN' metadata
            for (const auto& it : fMetadata) {
                if (startWith(it.first, "library_path")) {
                    fLibraryList.push_back(it.second);
                }
            }
        }
        if (meta_data2.find("include_pathnames") != meta_data2.end()) {
            fIncludePathnames = meta_data2["include_pathnames"];
        }
        
        fDSPSize = getInt(meta_data1, "size");
        fNumInputs = getInt(meta_data1, "inputs");
        fNumOutputs = getInt(meta_data1, "outputs");
        fSRIndex = getInt(meta_data1, "sr_index");
        fDSPProxy = false;
        
        // Prepare the fPathTable and init zone
        for (const auto& it : fUiItems) {
            std::string type = it.type;
            // Meta data declaration for input items
            if (isInput(type)) {
                ZoneParam* param = new ZoneParam();
                fPathInputTable.push_back(param);
                param->fZone = it.init;
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                ZoneParam* param = new ZoneParam();
                fPathOutputTable.push_back(param);
                param->fZone = REAL(0);
            }
        }
    }
    
    virtual ~JSONUIDecoderReal()
    {
        for (const auto& it : fPathInputTable) {
            delete it;
        }
        for (const auto& it : fPathOutputTable) {
            delete it;
        }
    }
    
    void metadata(Meta* m) override
    {
        for (const auto& it : fMetadata) {
            m->declare(it.first.c_str(), it.second.c_str());
        }
    }
    
    void metadata(MetaGlue* m) override
    {
        for (const auto& it : fMetadata) {
            m->declare(m->metaInterface, it.first.c_str(), it.second.c_str());
        }
    }
    
    void resetUserInterface() override
    {
        int item = 0;
        for (const auto& it : fUiItems) {
            if (isInput(it.type)) {
                static_cast<ZoneParam*>(fPathInputTable[item++])->fZone = it.init;
            }
        }
    }
    
    void resetUserInterface(char* memory_block, Soundfile* defaultsound = nullptr) override
    {
        for (const auto& it : fUiItems) {
            int index = it.index;
            if (isInput(it.type)) {
                *REAL_ADR(index) = it.init;
            } else if (isSoundfile(it.type)) {
                if (*SOUNDFILE_ADR(index) == nullptr) {
                    *SOUNDFILE_ADR(index) = defaultsound;
                }
            }
        }
    }
    
    int getSampleRate(char* memory_block) override
    {
        return *reinterpret_cast<int*>(&memory_block[fSRIndex]);
    }
    
    void setupDSPProxy(UI* ui_interface, char* memory_block) override
    {
        if (!fDSPProxy) {
            fDSPProxy = true;
            int countIn = 0;
            int countOut = 0;
            for (const auto& it : fUiItems) {
                std::string type = it.type;
                int index = it.index;
                if (isInput(type)) {
                    fPathInputTable[countIn++]->setReflectZoneFun([=](FAUSTFLOAT value) { *REAL_ADR(index) = REAL(value); });
                } else if (isOutput(type)) {
                    fPathOutputTable[countOut++]->setModifyZoneFun([=]() { return FAUSTFLOAT(*REAL_ADR(index)); });
                }
            }
        }
        
        // Setup soundfile in any case
        for (const auto& it : fUiItems) {
            if (isSoundfile(it.type)) {
                ui_interface->addSoundfile(it.label.c_str(), it.url.c_str(), SOUNDFILE_ADR(it.index));
            }
        }
    }
    
    bool hasDSPProxy() override { return fDSPProxy; }
  
    void buildUserInterface(UI* ui_interface) override
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = STRDUP(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        int countIn = 0;
        int countOut = 0;
        int countSound = 0;
        
        for (const auto& it : fUiItems) {
            
            std::string type = it.type;
            REAL init = REAL(it.init);
            REAL min = REAL(it.fmin);
            REAL max = REAL(it.fmax);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(&static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(&static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                ui_interface->openHorizontalBox(it.label.c_str());
            } else if (type == "vgroup") {
                ui_interface->openVerticalBox(it.label.c_str());
            } else if (type == "tgroup") {
                ui_interface->openTabBox(it.label.c_str());
            } else if (type == "vslider") {
                ui_interface->addVerticalSlider(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "hslider") {
                ui_interface->addHorizontalSlider(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "checkbox") {
                ui_interface->addCheckButton(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone);
            } else if (type == "soundfile") {
                // Nothing
            } else if (type == "hbargraph") {
                ui_interface->addHorizontalBargraph(it.label.c_str(), &static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, min, max);
            } else if (type == "vbargraph") {
                ui_interface->addVerticalBargraph(it.label.c_str(), &static_cast<ZoneParam*>(fPathOutputTable[countOut])->fZone, min, max);
            } else if (type == "nentry") {
                ui_interface->addNumEntry(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone, init, min, max, step);
            } else if (type == "button") {
                ui_interface->addButton(it.label.c_str(), &static_cast<ZoneParam*>(fPathInputTable[countIn])->fZone);
            } else if (type == "close") {
                ui_interface->closeBox();
            }
            
            if (isInput(type)) {
                countIn++;
            } else if (isOutput(type)) {
                countOut++;
            } else if (isSoundfile(type)) {
                countSound++;
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    void buildUserInterface(UI* ui_interface, char* memory_block) override
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = STRDUP(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        for (const auto& it : fUiItems) {
            
            std::string type = it.type;
            int index = it.index;
            REAL init = REAL(it.init);
            REAL min = REAL(it.fmin);
            REAL max = REAL(it.fmax);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(REAL_ADR(index), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(REAL_ADR(index), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    REAL_UI(ui_interface)->declare(0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                REAL_UI(ui_interface)->openHorizontalBox(it.label.c_str());
            } else if (type == "vgroup") {
                REAL_UI(ui_interface)->openVerticalBox(it.label.c_str());
            } else if (type == "tgroup") {
                REAL_UI(ui_interface)->openTabBox(it.label.c_str());
            } else if (type == "vslider") {
                REAL_UI(ui_interface)->addVerticalSlider(it.label.c_str(), REAL_ADR(index), init, min, max, step);
            } else if (type == "hslider") {
                REAL_UI(ui_interface)->addHorizontalSlider(it.label.c_str(), REAL_ADR(index), init, min, max, step);
            } else if (type == "checkbox") {
                REAL_UI(ui_interface)->addCheckButton(it.label.c_str(), REAL_ADR(index));
            } else if (type == "soundfile") {
                REAL_UI(ui_interface)->addSoundfile(it.label.c_str(), it.url.c_str(), SOUNDFILE_ADR(index));
            } else if (type == "hbargraph") {
                REAL_UI(ui_interface)->addHorizontalBargraph(it.label.c_str(), REAL_ADR(index), min, max);
            } else if (type == "vbargraph") {
                REAL_UI(ui_interface)->addVerticalBargraph(it.label.c_str(), REAL_ADR(index), min, max);
            } else if (type == "nentry") {
                REAL_UI(ui_interface)->addNumEntry(it.label.c_str(), REAL_ADR(index), init, min, max, step);
            } else if (type == "button") {
                REAL_UI(ui_interface)->addButton(it.label.c_str(), REAL_ADR(index));
            } else if (type == "close") {
                REAL_UI(ui_interface)->closeBox();
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    void buildUserInterface(UIGlue* ui_interface, char* memory_block) override
    {
        // MANDATORY: to be sure floats or double are correctly parsed
        char* tmp_local = setlocale(LC_ALL, nullptr);
        if (tmp_local != NULL) {
            tmp_local = STRDUP(tmp_local);
        }
        setlocale(LC_ALL, "C");
        
        for (const auto& it : fUiItems) {
            
            std::string type = it.type;
            int index = it.index;
            REAL init = REAL(it.init);
            REAL min = REAL(it.fmin);
            REAL max = REAL(it.fmax);
            REAL step = REAL(it.step);
            
            // Meta data declaration for input items
            if (isInput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, REAL_EXT_ADR(index), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for output items
            else if (isOutput(type)) {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, REAL_EXT_ADR(index), it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            // Meta data declaration for group opening or closing
            else {
                for (size_t i = 0; i < it.meta.size(); i++) {
                    ui_interface->declare(ui_interface->uiInterface, 0, it.meta[i].first.c_str(), it.meta[i].second.c_str());
                }
            }
            
            if (type == "hgroup") {
                ui_interface->openHorizontalBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "vgroup") {
                ui_interface->openVerticalBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "tgroup") {
                ui_interface->openTabBox(ui_interface->uiInterface, it.label.c_str());
            } else if (type == "vslider") {
                ui_interface->addVerticalSlider(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(index), init, min, max, step);
            } else if (type == "hslider") {
                ui_interface->addHorizontalSlider(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(index), init, min, max, step);
            } else if (type == "checkbox") {
                ui_interface->addCheckButton(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(index));
            } else if (type == "soundfile") {
                ui_interface->addSoundfile(ui_interface->uiInterface, it.label.c_str(), it.url.c_str(), SOUNDFILE_ADR(index));
            } else if (type == "hbargraph") {
                ui_interface->addHorizontalBargraph(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(index), min, max);
            } else if (type == "vbargraph") {
                ui_interface->addVerticalBargraph(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(index), min, max);
            } else if (type == "nentry") {
                ui_interface->addNumEntry(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(index), init, min, max, step);
            } else if (type == "button") {
                ui_interface->addButton(ui_interface->uiInterface, it.label.c_str(), REAL_EXT_ADR(index));
            } else if (type == "close") {
                ui_interface->closeBox(ui_interface->uiInterface);
            }
        }
        
        if (tmp_local != NULL) {
            setlocale(LC_ALL, tmp_local);
            free(tmp_local);
        }
    }
    
    bool hasCompileOption(const std::string& option) override
    {
        std::istringstream iss(fCompileOptions);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            if (token == option) return true;
        }
        return false;
    }
    
    std::string getCompileOption(const std::string& option) override
    {
        std::istringstream iss(fCompileOptions);
        std::string token;
        while (std::getline(iss, token, ' ')) {
            if (token == option) {
                std::string res;
                iss >> res;
                return res;
            }
        }
        return "";
    }
    
    int getDSPSize() override { return fDSPSize; }
    std::string getName() override { return fName; }
    std::string getLibVersion() override { return fVersion; }
    std::string getCompileOptions() override { return fCompileOptions; }
    std::vector<std::string> getLibraryList() override { return fLibraryList; }
    std::vector<std::string> getIncludePathnames() override { return fIncludePathnames; }
    int getNumInputs() override { return fNumInputs; }
    int getNumOutputs() override { return fNumOutputs; }
    
    std::vector<ExtZoneParam*>& getInputControls() override
    {
        return fPathInputTable;
    }
    std::vector<ExtZoneParam*>& getOutputControls() override
    {
        return fPathOutputTable;
    }
    
};

// FAUSTFLOAT templated decoder

struct FAUST_API JSONUIDecoder : public JSONUIDecoderReal<FAUSTFLOAT>
{
    JSONUIDecoder(const std::string& json):JSONUIDecoderReal<FAUSTFLOAT>(json)
    {}
};

// Generic factory

static JSONUIDecoderBase* createJSONUIDecoder(const std::string& json)
{
    JSONUIDecoder decoder(json);
    if (decoder.hasCompileOption("-double")) {
        return new JSONUIDecoderReal<double>(json);
    } else {
        return new JSONUIDecoderReal<float>(json);
    }
}

#endif
/**************************  END  JSONUIDecoder.h **************************/

/**
 * proxy_dsp
 *
 * Minimal helper that exposes a DSP described only by its JSON UI. It owns a JSONUIDecoder,
 * reports the number of audio I/O channels, rebuilds the UI, and forwards lifecycle calls
 * (init/instanceInit/instanceConstants/instanceResetUserInterface/instanceClear) to keep the
 * decoder state in sync with a real DSP that may live elsewhere (remote process, different
 * address space, etc.).
 *
 * Ownership:
 * - Owns the JSONUIDecoder created from a JSON string or from an existing dsp instance.
 * - Does NOT own audio/MIDI resources; subclasses are expected to implement compute().
 *
 * Lifecycle expectations:
 * - instanceConstants stores the sample rate locally (for subclasses to query getSampleRate()).
 * - instanceResetUserInterface resets the decoder UI so control defaults match the real DSP.
 * - compute() is left abstract; concrete subclasses implement the transport to the real DSP.
 *
 * Cloning:
 * - clone() duplicates only the decoder JSON; subclasses typically override it to clone their
 *   transport state as well.
 */
class proxy_dsp : public ::dsp {

    protected:
    
        JSONUIDecoder* fDecoder;
        int fSampleRate;
    
        void init(const std::string& json)
        {
            fDecoder = new JSONUIDecoder(json);
            fSampleRate = -1;
        }
        
    public:
    
        proxy_dsp():fDecoder(nullptr), fSampleRate(-1)
        {}
    
        proxy_dsp(const std::string& json)
        {
            init(json);
        }
          
        proxy_dsp(dsp* dsp)
        {
            JSONUI builder(dsp->getNumInputs(), dsp->getNumOutputs());
            dsp->metadata(&builder);
            dsp->buildUserInterface(&builder);
            fSampleRate = dsp->getSampleRate();
            fDecoder = new JSONUIDecoder(builder.JSON());
        }
      
        virtual ~proxy_dsp()
        {
            delete fDecoder;
        }
    
        virtual int getNumInputs() { return fDecoder->fNumInputs; }
        virtual int getNumOutputs() { return fDecoder->fNumOutputs; }
        
        virtual void buildUserInterface(UI* ui) { fDecoder->buildUserInterface(ui); }
        
        // To possibly implement in a concrete proxy dsp 
        virtual void init(int sample_rate)
        {
            instanceInit(sample_rate);
        }
        virtual void instanceInit(int sample_rate)
        {
            instanceConstants(sample_rate);
            instanceResetUserInterface();
            instanceClear();
        }
        virtual void instanceConstants(int sample_rate) { fSampleRate = sample_rate; }
        virtual void instanceResetUserInterface() { fDecoder->resetUserInterface(); }
        virtual void instanceClear() {}
    
        virtual int getSampleRate() { return fSampleRate; }
    
        virtual proxy_dsp* clone() { return new proxy_dsp(fDecoder->fJSON); }
        virtual void metadata(Meta* m) { fDecoder->metadata(m); }
    
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {}
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) {} 
        
};

/**
 * This class allows a 'decorator' like layer to smooth control changes over a
 * given duration (expressed in seconds) before applying them to the decorated dsp.
 *
 * Design notes
 * - Goal: wraps an existing dsp instance and transparently smooths its input controllers
 *   whenever any control value changes.
 *
 * - Inputs vs outputs: only input controls (sliders, buttons, nentry, checkbox) are smoothed.
 *   Bargraphs and soundfiles are ignored by the smoothing logic but remain exposed via the
 *   JSONUIDecoder UI.
 *
 * - Smoothing model: linear or exponential per control over `fSmoothingSamples` samples.
 *   Linear: per-sample `increment = (target - current) / duration`, exact snap at end.
 *   Exponential: per-sample decay `current = target + (current - target) * alpha` where
 *   `alpha` is chosen so the residual after the duration is ~1e-4 (-80 dB), then snapped exact at end.
 *   If `fSmoothingSamples <= 0`, values jump immediately to targets. Buttons and checkboxes are
 *   treated as toggles and jump directly without smoothing.
 *
 * - Smoothing strategy: supplied as a template parameter (LinearSmoother and ExpSmoother provided).
 *
 * - Smoothing configuration: callers pass a duration in seconds; the class converts it to a
 *   sample count on instanceConstants(), then uses that count for ramp scheduling.
 *   Callers can also choose the smoothing step size (in samples) to control how often controller
 *   values are updated (default 1 sample).
 *
 * - Triggering: the ramp setup occurs at the beginning of each compute() (both variants). If no
 *   control has changed since the previous compute, no new ramp is created and processing runs at
 *   full block size. When a ramp is active, processing uses blocks of `smoothingStep` samples
 *   (default 1, max step is clamped to the current audio block size in `compute`) until the ramp ends,
 *   then resumes block processing for the remaining frames.
 *
 * - State/ownership: the class owns its JSONUIDecoder; the wrapped dsp is owned by decorator_dsp.
 *   clone() deep-clones both the decoder JSON and the wrapped dsp and preserves the smoothing
 *   duration.
 *
 * - Reset behaviour: instanceResetUserInterface() resets both the wrapped dsp UI and the decoder
 *   state, then realigns smoothing buffers to the reset control values. instanceClear() stops any
 *   ongoing ramp.
 * - API expectations: smoothing is configured in seconds (not ms). Callers must supply the desired
 *   duration in seconds when constructing the decorator.
 */

// Base class for Smoother
// - finish: snaps to exact targets and clears any stored steps.
struct Smoother {
    
    // Snap to targets at ramp completion and clear step deltas.
    template <typename ControlContainer>
    void finish(ControlContainer& controls)
    {
        size_t count = controls.size();
        for (size_t i = 0; i < count; ++i) {
            controls[i].fCurrent = controls[i].fTarget;
            *controls[i].fDspZone = controls[i].fCurrent;
            controls[i].fStep = FAUSTFLOAT(0);
        }
    }
};

// Linear ramp: distributes the delta evenly across durationSamples.
// - start: computes per-control steps = (target - current) / durationSamples and sets remaining.
// - step: adds one step to each control, writes the zone.
struct LinearSmoother : public Smoother {
    template <typename ControlContainer>
    void start(ControlContainer& controls, int durationSamples, int& remaining)
    {
        size_t count = controls.size();
        remaining = durationSamples;
        if (remaining <= 0) {
            for (size_t i = 0; i < count; ++i) {
                controls[i].fStep = FAUSTFLOAT(0);
            }
            remaining = 0;
        } else {
            for (size_t i = 0; i < count; ++i) {
                controls[i].fStep = (controls[i].fTarget - controls[i].fCurrent) / FAUSTFLOAT(durationSamples);
            }
        }
    }
    
    // Advance one sample worth of linear interpolation and write zones.
    template <typename ControlContainer>
    void step(ControlContainer& controls, int stepSamples)
    {
        size_t count = controls.size();
        for (size_t i = 0; i < count; ++i) {
            controls[i].fCurrent += controls[i].fStep * FAUSTFLOAT(stepSamples);
            *controls[i].fDspZone = controls[i].fCurrent;
        }
    }
    
};

// Exponential approach toward target: current = target + (current - target) * alpha.
// - start: precomputes alpha so the residual after durationSamples is ~1e-4 (-80 dB) and sets remaining
//   (the -80 dB tail is an arbitrary choice; change if you want a different decay depth).
// - step: decays current toward target using alpha, writes the zone.
struct ExpSmoother : public Smoother {
    double fAlpha;
    
    ExpSmoother():fAlpha(0) {}
    
    // Precompute alpha for exponential decay over durationSamples; empty steps vector is kept for API symmetry.
    template <typename ControlContainer>
    void start(ControlContainer& controls, int durationSamples, int& remaining)
    {
        if (durationSamples <= 0) {
            remaining = 0;
            fAlpha = 0;
        } else {
            remaining = durationSamples;
            // Choose alpha so that the residual after 'durationSamples' steps is around -80 dB (~1e-4).
            const double targetResidual = 1e-4;
            fAlpha = std::exp(std::log(targetResidual) / double(durationSamples));
        }
        // Keep step/scratch cleared.
        for (size_t i = 0; i < controls.size(); ++i) {
            controls[i].fStep = FAUSTFLOAT(0);
        }
    }
    
    // Apply exponential decay for one sample and write zones.
    template <typename ControlContainer>
    void step(ControlContainer& controls, int stepSamples)
    {
        size_t count = controls.size();
        double alpha = std::pow(fAlpha, stepSamples);
        for (size_t i = 0; i < count; ++i) {
            controls[i].fCurrent = controls[i].fTarget
                                    + (controls[i].fCurrent - controls[i].fTarget) * FAUSTFLOAT(alpha);
            *controls[i].fDspZone = controls[i].fCurrent;
        }
    }
    
};

template <typename Smoother>
class smoothing_dsp : public decorator_dsp {

    protected:
    
        JSONUIDecoder* fDecoder;
        double fSmoothingSec;    // Smoothing duration in seconds (construction-time parameter)
        int fSmoothingSamples;   // Smoothing duration in samples (derived from samplerate)
        int fSmoothingStep;      // Number of samples between smoothing updates
        int fRemaining;          // Remaining samples in current smoothing ramp
        using zone_param = JSONUIDecoderReal<FAUSTFLOAT>::ZoneParam;

        struct ControlData {
            FAUSTFLOAT* fDspZone;   // Pointer to wrapped DSP control zone
            bool fIsToggle;         // True for buttons/checkboxes (no smoothing)
            FAUSTFLOAT fCurrent;    // Working value used by smoothing
            FAUSTFLOAT fTarget;     // Desired value from decoder
            FAUSTFLOAT fStep;       // Linear step (or scratch for other smoothers)
        };
        
        // Control data grouped in a single container
        std::vector<ControlData> fControls;
        std::vector<FAUSTFLOAT*> fDecoderZones;    // Fast access to decoder-side control zones
        std::vector<FAUSTFLOAT*> fInputPtrs;
        std::vector<FAUSTFLOAT*> fOutputPtrs;
        
        Smoother fSmoother;
        
        // Helper UI that only collects input control zones from the wrapped dsp.
        struct ControlCollector : public GenericUI {
            
            std::vector<FAUSTFLOAT*> fInputs;
            std::vector<bool> fToggles;
            
            void addButton(const char* /*label*/, FAUSTFLOAT* zone)
            {
                fInputs.push_back(zone);
                fToggles.push_back(true);
            }
            void addCheckButton(const char* /*label*/, FAUSTFLOAT* zone)
            {
                fInputs.push_back(zone);
                fToggles.push_back(true);
            }
            void addVerticalSlider(const char* /*label*/, FAUSTFLOAT* zone, FAUSTFLOAT /*init*/, FAUSTFLOAT /*fmin*/, FAUSTFLOAT /*fmax*/, FAUSTFLOAT /*step*/)
            {
                fInputs.push_back(zone);
                fToggles.push_back(false);
            }
            void addHorizontalSlider(const char* /*label*/, FAUSTFLOAT* zone, FAUSTFLOAT /*init*/, FAUSTFLOAT /*fmin*/, FAUSTFLOAT /*fmax*/, FAUSTFLOAT /*step*/)
            {
                fInputs.push_back(zone);
                fToggles.push_back(false);
            }
            void addNumEntry(const char* /*label*/, FAUSTFLOAT* zone, FAUSTFLOAT /*init*/, FAUSTFLOAT /*fmin*/, FAUSTFLOAT /*fmax*/, FAUSTFLOAT /*step*/)
            {
                fInputs.push_back(zone);
                fToggles.push_back(false);
            }
        };
        
        size_t getControlCount() const
        {
             return fControls.size();
        }

        // Collect raw zones from the wrapped dsp UI for direct writes.
        void collectControlZones(::dsp* dsp)
        {
            ControlCollector collector;
            dsp->buildUserInterface(&collector);
            size_t count = collector.fInputs.size();
            fControls.resize(count);
            for (size_t i = 0; i < count; ++i) {
                fControls[i].fDspZone = collector.fInputs[i];
                fControls[i].fIsToggle = collector.fToggles[i];
                fControls[i].fCurrent = FAUSTFLOAT(0);
                fControls[i].fTarget = FAUSTFLOAT(0);
                fControls[i].fStep = FAUSTFLOAT(0);
            }
        }
        
        // Cache decoder-side control zones for fast access in smoothing.
        void buildDecoderZones()
        {
            auto& inputs = fDecoder->getInputControls();
            fDecoderZones.resize(inputs.size());
            for (size_t i = 0; i < inputs.size(); ++i) {
                fDecoderZones[i] = &static_cast<zone_param*>(inputs[i])->fZone;
            }
        }
        
        // Reinitialize smoothing buffers and push initial values to the wrapped dsp.
        void resetSmoothingState()
        {
            size_t count = getControlCount();
            for (size_t i = 0; i < count; ++i) {
                FAUSTFLOAT value = *fDecoderZones[i];
                fControls[i].fCurrent = value;
                fControls[i].fTarget = value;
                fControls[i].fStep = FAUSTFLOAT(0);
                *fControls[i].fDspZone = value;
            }
            fRemaining = 0;
        }
        
        // Detect controller changes and configure ramp data for the selected smoothing strategy.
        // - Scans decoder inputs; toggles (buttons/checkboxes) are applied immediately.
        // - If smoothing duration is zero, applies new targets instantly.
        // - Otherwise delegates ramp setup to the active smoother.
        void setupSmoothing()
        {
            // Detect control changes, configure ramps, or jump immediately if smoothing is disabled
            bool changed = false;
            size_t count = getControlCount();
            for (size_t i = 0; i < count; ++i) {
                FAUSTFLOAT next = *fDecoderZones[i];
                assert(i < fControls.size());
                ControlData& ctl = fControls[i];
                if (next != ctl.fTarget) {
                    // Scans decoder inputs; toggles (buttons/checkboxes) are applied immediately.
                    if (ctl.fIsToggle) {
                        // Buttons/checkboxes jump directly.
                        ctl.fTarget = next;
                        ctl.fCurrent = next;
                        *ctl.fDspZone = next;
                    } else {
                        // Range controllers will be smoothed
                        ctl.fTarget = next;
                        changed = true;
                    }
                }
            }
    
            if (!changed) {
                return;
            }
    
            // - If smoothing duration is zero, applies new targets instantly.
            if (fSmoothingSamples <= 0) {
                for (size_t i = 0; i < count; ++i) {
                    ControlData& ctl = fControls[i];
                    ctl.fCurrent = ctl.fTarget;
                    ctl.fStep = FAUSTFLOAT(0);
                    *ctl.fDspZone = ctl.fCurrent;
                }
                fRemaining = 0;
                return;
            }
    
            // - Otherwise delegates ramp setup to the active smoother.
            fSmoother.start(fControls, fSmoothingSamples, fRemaining);
            if (fRemaining <= 0) {
                fSmoother.finish(fControls);
                fRemaining = 0;
            }
        }
        
        // Convert seconds-based duration into samples using current samplerate.
        void updateSmoothingSamples()
        {
            fSmoothingSamples = (fSmoothingSec > 0) ? (fSmoothingSec * double(decorator_dsp::getSampleRate())) : 0;
        }
        
        // Initialize decoder and smoothing state using an existing dsp instance.
        void init(const std::string& json, ::dsp* dsp, double smoothing_sec, int smoothing_step)
        {
            fDecoder = new JSONUIDecoder(json);
            collectControlZones(dsp);
            buildDecoderZones();
            fInputPtrs.assign(getNumInputs(), nullptr);
            fOutputPtrs.assign(getNumOutputs(), nullptr);
            fSmoothingSec = smoothing_sec;
            fSmoothingStep = std::max<int>(1, smoothing_step);
            fSmoothingSamples = 0;
            fRemaining = 0;
            resetSmoothingState();
        }
    
        // Helper to position per-channel input/output pointers at a given frame offset.
        void setIO(std::vector<FAUSTFLOAT*>& input_ptrs,
                   std::vector<FAUSTFLOAT*>& output_ptrs,
                   FAUSTFLOAT** inputs,
                   FAUSTFLOAT** outputs,
                   int frame_offset)
        {
            int num_inputs = getNumInputs();
            int num_outputs = getNumOutputs();
            for (int chan = 0; chan < num_inputs; ++chan) {
                input_ptrs[chan] = inputs ? inputs[chan] + frame_offset : nullptr;
            }
            for (int chan = 0; chan < num_outputs; ++chan) {
                output_ptrs[chan] = outputs ? outputs[chan] + frame_offset : nullptr;
            }
        }
        
    public:
    
        /**
         * Wrap an existing dsp and smooth its input controls.
         *
         * smoothing_sec : duration in seconds for non-toggle controls; <= 0 disables smoothing (controls jump).
         * smoothing_step: number of samples between smoothing updates (minimum 1). Use values > 1 to reduce control update cost
         *                 when audio blocks are large; max step is clamped to the current audio block size in compute().
         *
         * The constructor rebuilds the JSON UI from `dsp` so callers do not need to provide one explicitly.
         */
        // Build from a concrete dsp, selecting smoothing duration (seconds).
        smoothing_dsp(::dsp* dsp, double smoothing_sec = 0, int smoothing_step = 1)
        : decorator_dsp(dsp), fDecoder(nullptr), fSmoothingSec(smoothing_sec), fSmoothingSamples(0),
          fSmoothingStep(std::max<int>(1, smoothing_step)), fRemaining(0),
          fSmoother()
        {
            // Build JSON description from the wrapped dsp and initialize smoothing
            JSONUI builder(dsp->getNumInputs(), dsp->getNumOutputs());
            dsp->metadata(&builder);
            dsp->buildUserInterface(&builder);
            init(builder.JSON(), dsp, smoothing_sec, smoothing_step);
        }
        
        /**
         * Wrap an existing dsp using a precomputed JSON UI description.
         *
         * Use this form when the JSON UI must match an external/remote representation exactly (for example, when proxying a DSP
         * hosted elsewhere). The smoothing parameters behave the same as the other constructor.
         *
         * smoothing_sec : duration in seconds for non-toggle controls; <= 0 disables smoothing (controls jump).
         * smoothing_step: number of samples between smoothing updates (minimum 1). Use values > 1 to reduce control update cost
         *                 when audio blocks are large; max step is clamped to the current audio block size in compute().
         */
        // Build from explicit JSON and a concrete dsp, with smoothing settings.
        smoothing_dsp(const std::string& json, ::dsp* dsp, double smoothing_sec = 0, int smoothing_step = 1)
        : decorator_dsp(dsp), fDecoder(nullptr), fSmoothingSec(smoothing_sec), fSmoothingSamples(0),
          fSmoothingStep(std::max<int>(1, smoothing_step)), fRemaining(0),
          fSmoother()
        {
            init(json, dsp, smoothing_sec, smoothing_step);
        }
    
        // Release decoder (wrapped dsp is owned by decorator_dsp).
        virtual ~smoothing_dsp()
        {
            delete fDecoder;
        }
        
        // Expose JSON UI while preserving smoothing indirection.
        virtual void buildUserInterface(UI* ui) { fDecoder->buildUserInterface(ui); }
        
        // Lifecycle mirrors proxy_dsp but keeps smoothing state coherent
        virtual void init(int sample_rate)
        {
            decorator_dsp::init(sample_rate);
            instanceInit(sample_rate);
        }
        virtual void instanceInit(int sample_rate)
        {
            instanceConstants(sample_rate);
            instanceResetUserInterface();
            instanceClear();
        }
    
        // Store samplerate while forwarding constants to wrapped dsp.
        virtual void instanceConstants(int sample_rate)
        {
            decorator_dsp::instanceConstants(sample_rate);
            updateSmoothingSamples();
        }
        // Reset both decoder UI and wrapped dsp UI, then realign smoothing buffers.
        virtual void instanceResetUserInterface()
        {
            decorator_dsp::instanceResetUserInterface();
            fDecoder->resetUserInterface();
            resetSmoothingState();
        }
        // Clear wrapped dsp state and stop any ongoing ramp.
        virtual void instanceClear()
        {
            decorator_dsp::instanceClear();
            fRemaining = 0;
        }
    
        // Clone decoder JSON, wrapped dsp, and smoothing strategy
        virtual smoothing_dsp* clone()
        {
            return new smoothing_dsp(fDecoder->fJSON, fDSP->clone(), fSmoothingSec, fSmoothingStep);
        }
        // Forward metadata from decoder
        virtual void metadata(Meta* m) { fDecoder->metadata(m); }
    
        // Top-level audio processing: apply pending control ramps using the configured smoothing step, then finish the block.
        virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            // Run control smoothing then process audio.
            // If a ramp is active, we interleave control updates with audio in blocks of `fSmoothingStep` samples.
            // Once smoothing completes, remaining frames (if any) are processed in a single block.
            
            setupSmoothing();
            
            if (fRemaining <= 0) {
                // No smoothing needed, so regular compute
                decorator_dsp::compute(count, inputs, outputs);
            } else {
                
                // Smoothing
                int offset = 0;
                while (offset < count && fRemaining > 0) {
                    // Update controls for this smoothing step and advance the ramp
                    int stepSize = std::min<int>({fSmoothingStep, fRemaining, count - offset});
                    fSmoother.step(fControls, stepSize);
                    fRemaining -= stepSize;
                    if (fRemaining == 0) {
                        // Ensure exact target values when ramp ends
                        fSmoother.finish(fControls);
                    }
                    // Compute this block with updated controls
                    setIO(fInputPtrs, fOutputPtrs, inputs, outputs, offset);
                    decorator_dsp::compute(stepSize, fInputPtrs.data(), fOutputPtrs.data());
                    offset += stepSize;
                }
                
                // No active ramp: process remaining frames in one call
                if (offset < count) {
                    setIO(fInputPtrs, fOutputPtrs, inputs, outputs, offset);
                    decorator_dsp::compute(count - offset, fInputPtrs.data(), fOutputPtrs.data());
                }
            }
        }
    
        virtual void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) { compute(count, inputs, outputs); }
        
};

using smoothing_dsp_linear = smoothing_dsp<LinearSmoother>;
using smoothing_dsp_exp = smoothing_dsp<ExpSmoother>;

#endif
/************************** END proxy-dsp.h **************************/
        

/************************** BEGIN JSONControl.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 *************************************************************************/

#ifndef __JSON_CONTROL__
#define __JSON_CONTROL__

#include <string>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

struct FAUST_API JSONControl {
    
    virtual std::string getJSON() { return ""; }

    virtual void setParamValue(const std::string& path, FAUSTFLOAT value) {}

    virtual FAUSTFLOAT getParamValue(const std::string& path) { return 0; }
    
    virtual ~JSONControl()
    {}
    
};

#endif
/**************************  END  JSONControl.h **************************/

#define kActiveVoice    0
#define kFreeVoice     -1
#define kReleaseVoice  -2
#define kLegatoVoice   -3
#define kNoVoice       -4

#define VOICE_STOP_LEVEL 0.00003162  // -90 db
#define MIX_BUFFER_SIZE  4096

/**
 * Allows to control zones in a grouped manner.
 */
class GroupUI : public GUI, public PathBuilder {

    private:

        // Map to associate labels with UI group items
        std::map<std::string, uiGroupItem*> fLabelZoneMap;

        // Insert a zone into the map based on the label folloing the freq/gain/gate polyphonic convention
        void insertMap(std::string label, FAUSTFLOAT* zone)
        {
            if (!MapUI::endsWith(label, "/gate")
                && !MapUI::endsWith(label, "/freq")
                && !MapUI::endsWith(label, "/key")
                && !MapUI::endsWith(label, "/gain")
                && !MapUI::endsWith(label, "/vel")
                && !MapUI::endsWith(label, "/velocity")) {

                // Groups all controllers except 'freq/key', 'gate', and 'gain/vel|velocity'
                if (fLabelZoneMap.find(label) != fLabelZoneMap.end()) {
                    fLabelZoneMap[label]->addZone(zone);
                } else {
                    fLabelZoneMap[label] = new uiGroupItem(this, zone);
                }
            }
        }

        uiCallbackItem* fPanic;

    public:

        GroupUI(FAUSTFLOAT* zone, uiCallback cb, void* arg)
        {
            fPanic = new uiCallbackItem(this, zone, cb, arg);
        }
    
        virtual ~GroupUI()
        {
            // 'fPanic' is kept and deleted in GUI, so do not delete here
        }

        // -- widget's layouts
        void openTabBox(const char* label)
        {
            pushLabel(label);
        }
        void openHorizontalBox(const char* label)
        {
            pushLabel(label);
        }
        void openVerticalBox(const char* label)
        {
            pushLabel(label);
        }
        void closeBox()
        {
            popLabel();
        }

        // -- active widgets
        void addButton(const char* label, FAUSTFLOAT* zone)
        {
            insertMap(buildPath(label), zone);
        }
        void addCheckButton(const char* label, FAUSTFLOAT* zone)
        {
            insertMap(buildPath(label), zone);
        }
        void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }
        void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }
        void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
        {
            insertMap(buildPath(label), zone);
        }

        // -- passive widgets
        void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            insertMap(buildPath(label), zone);
        }
        void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT fmin, FAUSTFLOAT fmax)
        {
            insertMap(buildPath(label), zone);
        }

};

/**
 * One voice of polyphony.
 */
struct dsp_voice : public MapUI, public decorator_dsp {
    
    typedef std::function<double(int)> TransformFunction;
  
    // Convert MIDI note to frequency
    static double midiToFreq(double note)
    {
        return 440.0 * std::pow(2.0, (note-69.0)/12.0);
    }
    
    // Voice state and properties
    int fCurNote;                       // Current playing note pitch
    int fNextNote;                      // In kLegatoVoice state, next note to play
    int fNextVel;                       // In kLegatoVoice state, next velocity to play
    int fDate;                          // KeyOn date
    FAUSTFLOAT fLevel;                  // Last audio block level
    std::vector<std::string> fGatePath; // Paths of 'gate' control
    std::vector<std::string> fGainPath; // Paths of 'gain/vel|velocity' control
    std::vector<std::string> fFreqPath; // Paths of 'freq/key' control
    TransformFunction        fKeyFun;   // MIDI key to freq conversion function
    TransformFunction        fVelFun;   // MIDI velocity to gain conversion function
    
    FAUSTFLOAT** fInputsSlice;
    FAUSTFLOAT** fOutputsSlice;
 
    dsp_voice(::dsp* dsp):decorator_dsp(dsp)
    {
        // Default conversion functions
        fVelFun = [](int velocity) { return double(velocity)/127.0; };
        fKeyFun = [](int pitch) { return midiToFreq(pitch); };
        dsp->buildUserInterface(this);
        fCurNote = kFreeVoice;
        fNextNote = fNextVel = -1;
        fLevel = FAUSTFLOAT(0);
        fDate = 0;
        extractPaths(fGatePath, fFreqPath, fGainPath);
    }
    virtual ~dsp_voice()
    {}
    
    // Compute a slice of audio
    void computeSlice(int offset, int slice, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        FAUSTFLOAT** inputsSlice = static_cast<FAUSTFLOAT**>(alloca(sizeof(FAUSTFLOAT*) * getNumInputs()));
        for (int chan = 0; chan < getNumInputs(); chan++) {
            inputsSlice[chan] = &(inputs[chan][offset]);
        }
        FAUSTFLOAT** outputsSlice = static_cast<FAUSTFLOAT**>(alloca(sizeof(FAUSTFLOAT*) * getNumOutputs()));
        for (int chan = 0; chan < getNumOutputs(); chan++) {
            outputsSlice[chan] = &(outputs[chan][offset]);
        }
        compute(slice, inputsSlice, outputsSlice);
    }
    
    // Compute audio in legato mode
    void computeLegato(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
    {
        int slice = count/2;
        
        // Reset envelops
        for (size_t i = 0; i < fGatePath.size(); i++) {
            setParamValue(fGatePath[i], FAUSTFLOAT(0));
        }
        
        // Compute current voice on half buffer
        computeSlice(0, slice, inputs, outputs);
         
        // Start next keyOn
        keyOn(fNextNote, fNextVel);
        
        // Compute on second half buffer
        computeSlice(slice, slice, inputs, outputs);
    }

    // Extract control paths from fullpath map
    void extractPaths(std::vector<std::string>& gate, std::vector<std::string>& freq, std::vector<std::string>& gain)
    {
        // Keep gain/vel|velocity, freq/key and gate labels
        for (const auto& it : getFullpathMap()) {
            std::string path = it.first;
            if (endsWith(path, "/gate")) {
                gate.push_back(path);
            } else if (endsWith(path, "/freq")) {
                fKeyFun = [](int pitch) { return midiToFreq(pitch); };
                freq.push_back(path);
            } else if (endsWith(path, "/key")) {
                fKeyFun = [](int pitch) { return pitch; };
                freq.push_back(path);
            } else if (endsWith(path, "/gain")) {
                fVelFun = [](int velocity) { return double(velocity)/127.0; };
                gain.push_back(path);
            } else if (endsWith(path, "/vel") || endsWith(path, "/velocity")) {
                fVelFun = [](int velocity) { return double(velocity); };
                gain.push_back(path);
            }
        }
    }
    
    // Reset voice
    void reset()
    {
        init(getSampleRate());
    }
 
    // Clear instance state
    void instanceClear()
    {
        decorator_dsp::instanceClear();
        fCurNote = kFreeVoice;
        fNextNote = fNextVel = -1;
        fLevel = FAUSTFLOAT(0);
        fDate = 0;
    }
    
    // Keep 'pitch' and 'velocity' to fadeOut the current voice and start next one in the next buffer
    void keyOn(int pitch, int velocity, bool legato = false)
    {
        if (legato) {
            fNextNote = pitch;
            fNextVel = velocity;
        } else {
            keyOn(pitch, fVelFun(velocity));
        }
    }

    // KeyOn with normalized MIDI velocity [0..1]
    void keyOn(int pitch, double velocity)
    {
        for (size_t i = 0; i < fFreqPath.size(); i++) {
            setParamValue(fFreqPath[i], fKeyFun(pitch));
        }
        for (size_t i = 0; i < fGatePath.size(); i++) {
            setParamValue(fGatePath[i], FAUSTFLOAT(1));
        }
        for (size_t i = 0; i < fGainPath.size(); i++) {
            setParamValue(fGainPath[i], velocity);
        }
        
        fCurNote = pitch;
    }

    void keyOff(bool hard = false)
    {
        // No use of velocity for now...
        for (size_t i = 0; i < fGatePath.size(); i++) {
            setParamValue(fGatePath[i], FAUSTFLOAT(0));
        }
        
        if (hard) {
            // Immediately stop voice
            fCurNote = kFreeVoice;
        } else {
            // Release voice
            fCurNote = kReleaseVoice;
        }
    }

};

/**
 * A group of voices.
 */
struct dsp_voice_group {

    // GUI group for controlling voice parameters
    GroupUI fGroups;

    std::vector<dsp_voice*> fVoiceTable; // Individual voices
    ::dsp* fVoiceGroup;                  // Voices group to be used for GUI grouped control

    FAUSTFLOAT fPanic;  // Panic button value

    bool fVoiceControl; // Voice control mode
    bool fGroupControl; // Group control mode

    dsp_voice_group(uiCallback cb, void* arg, bool control, bool group)
        :fGroups(&fPanic, cb, arg),
        fVoiceGroup(0), fPanic(FAUSTFLOAT(0)),
        fVoiceControl(control), fGroupControl(group)
    {}

    virtual ~dsp_voice_group()
    {
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            delete fVoiceTable[i];
        }
        delete fVoiceGroup;
    }

    // Add a voice to the group
    void addVoice(dsp_voice* voice)
    {
        fVoiceTable.push_back(voice);
    }
        
    // Clear all voices from the group
    void clearVoices()
    {
        fVoiceTable.clear();
    }

    // Initialize the voice group
    void init()
    {
        // Groups all uiItem for a given path
        fVoiceGroup = new proxy_dsp(fVoiceTable[0]);
        fVoiceGroup->buildUserInterface(&fGroups);
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            fVoiceTable[i]->buildUserInterface(&fGroups);
        }
    }
    
    // Reset the user interface for each voice instance
    void instanceResetUserInterface()
    {
        for (size_t i = 0; i < fVoiceTable.size(); i++) {
            fVoiceTable[i]->instanceResetUserInterface();
        }
    }

    // Build the user interface for the voice group
    void buildUserInterface(UI* ui_interface)
    {
        if (fVoiceTable.size() > 1) {
            ui_interface->openTabBox("Polyphonic");

            // Grouped voices UI
            ui_interface->openVerticalBox("Voices");
            ui_interface->addButton("Panic", &fPanic);
            fVoiceGroup->buildUserInterface(ui_interface);
            ui_interface->closeBox();

            // If not grouped, also add individual voices UI
#ifdef DAISY_NO_RTTI
            if (!fGroupControl || ui_interface->isSoundUI()) {
#else
            if (!fGroupControl || dynamic_cast<SoundUIInterface*>(ui_interface)) {
#endif
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    char buffer[32];
                    snprintf(buffer, 32, ((fVoiceTable.size() < 8) ? "Voice%ld" : "V%ld"), long(i+1));
                    ui_interface->openHorizontalBox(buffer);
                    fVoiceTable[i]->buildUserInterface(ui_interface);
                    ui_interface->closeBox();
                }
            }

            ui_interface->closeBox();
        } else {
            fVoiceTable[0]->buildUserInterface(ui_interface);
        }
    }

};

/**
 * Base class for MIDI controllable polyphonic DSP.
 */
#ifdef EMCC
#endif

class dsp_poly : public decorator_dsp, public midi, public JSONControl {

    protected:
    
    #ifdef EMCC
        MapUI fMapUI;               // Map for UI control
        std::string fJSON;          // JSON representation of the UI
        midi_handler fMidiHandler;  // MIDI handler for the UI
        MidiUI fMIDIUI;             // MIDI UI for the DSP
    #endif
    
    public:
    
    #ifdef EMCC
        dsp_poly(::dsp* dsp):decorator_dsp(dsp), fMIDIUI(&fMidiHandler)
        {
            JSONUI jsonui(getNumInputs(), getNumOutputs());
            buildUserInterface(&jsonui);
            fJSON = jsonui.JSON(true);
            buildUserInterface(&fMapUI);
            buildUserInterface(&fMIDIUI);
        }
    #else
        dsp_poly(::dsp* dsp):decorator_dsp(dsp)
        {}
    #endif
    
        virtual ~dsp_poly() {}
    
        // Reimplemented for EMCC
    #ifdef EMCC
        virtual int getNumInputs() { return decorator_dsp::getNumInputs(); }
        virtual int getNumOutputs() { return decorator_dsp::getNumOutputs(); }
        virtual void buildUserInterface(UI* ui_interface) { decorator_dsp::buildUserInterface(ui_interface); }
        virtual int getSampleRate() { return decorator_dsp::getSampleRate(); }
        virtual void init(int sample_rate) { decorator_dsp::init(sample_rate); }
        virtual void instanceInit(int sample_rate) { decorator_dsp::instanceInit(sample_rate); }
        virtual void instanceConstants(int sample_rate) { decorator_dsp::instanceConstants(sample_rate); }
        virtual void instanceResetUserInterface() { decorator_dsp::instanceResetUserInterface(); }
        virtual void instanceClear() { decorator_dsp::instanceClear(); }
        virtual dsp_poly* clone() { return new dsp_poly(fDSP->clone()); }
        virtual void metadata(Meta* m) { decorator_dsp::metadata(m); }
    
        // Additional API
        std::string getJSON()
        {
            return fJSON;
        }
    
        virtual void setParamValue(const std::string& path, FAUSTFLOAT value)
        {
            fMapUI.setParamValue(path, value);
            GUI::updateAllGuis();
        }
        
        virtual FAUSTFLOAT getParamValue(const std::string& path) { return fMapUI.getParamValue(path); }

        virtual void computeJS(int count, uintptr_t inputs, uintptr_t outputs)
        {
            decorator_dsp::compute(count, reinterpret_cast<FAUSTFLOAT**>(inputs),reinterpret_cast<FAUSTFLOAT**>(outputs));
        }
    #endif
    
        virtual MapUI* keyOn(int channel, int pitch, int velocity)
        {
            return midi::keyOn(channel, pitch, velocity);
        }
        virtual void keyOff(int channel, int pitch, int velocity)
        {
            midi::keyOff(channel, pitch, velocity);
        }
        virtual void keyPress(int channel, int pitch, int press)
        {
            midi::keyPress(channel, pitch, press);
        }
        virtual void chanPress(int channel, int press)
        {
            midi::chanPress(channel, press);
        }
        virtual void ctrlChange(int channel, int ctrl, int value)
        {
            midi::ctrlChange(channel, ctrl, value);
        }
        virtual void ctrlChange14bits(int channel, int ctrl, int value)
        {
            midi::ctrlChange14bits(channel, ctrl, value);
        }
        virtual void pitchWheel(int channel, int wheel)
        {
        #ifdef EMCC
            fMIDIUI.pitchWheel(0., channel, wheel);
            GUI::updateAllGuis();
        #else
            midi::pitchWheel(channel, wheel);
        #endif
        }
        virtual void progChange(int channel, int pgm)
        {
            midi::progChange(channel, pgm);
        }
     
};

/**
 * Polyphonic DSP: groups a set of DSP to be played together or triggered by MIDI.
 *
 * All voices are preallocated by cloning the single DSP voice given at creation time.
 * Dynamic voice allocation is done in 'getFreeVoice'
 */
class mydsp_poly : public dsp_voice_group, public dsp_poly {

    private:

        FAUSTFLOAT** fMixBuffer;        // Intermediate buffer for mixing voices
        FAUSTFLOAT** fOutBuffer;        // Intermediate buffer for output
        midi_interface* fMidiHandler;   // The midi_interface the DSP is connected to
        int fDate;                      // Current date for managing voices
    
        // Fade out the audio in the buffer
        void fadeOut(int count, FAUSTFLOAT** outBuffer)
        {
            // FadeOut on half buffer
            for (int chan = 0; chan < getNumOutputs(); chan++) {
                double factor = 1., step = 1./double(count);
                for (int frame = 0; frame < count; frame++) {
                    outBuffer[chan][frame] *= factor;
                    factor -= step;
                }
            }
        }
    
        FAUSTFLOAT mixCheckVoice(int count, FAUSTFLOAT** mixBuffer, FAUSTFLOAT** outBuffer)
        {
            FAUSTFLOAT sumSquares = 0;
            int numOutputs = getNumOutputs();
            
            for (int chan = 0; chan < numOutputs; chan++) {
                FAUSTFLOAT* mixChannel = mixBuffer[chan];
                FAUSTFLOAT* outChannel = outBuffer[chan];
                for (int frame = 0; frame < count; frame++) {
                    FAUSTFLOAT sample = mixChannel[frame];
                    sumSquares += sample * sample;
                    outChannel[frame] += sample;
                }
            }
            
            // RMS is sqrt of mean of sum of squares across all samples in all channels
            FAUSTFLOAT meanSquare = sumSquares / (count * numOutputs);
            return std::sqrt(meanSquare);
        }
    
        // Mix the audio from the mix buffer to the output buffer
        void mixVoice(int count, FAUSTFLOAT** mixBuffer, FAUSTFLOAT** outBuffer)
        {
            for (int chan = 0; chan < getNumOutputs(); chan++) {
                FAUSTFLOAT* mixChannel = mixBuffer[chan];
                FAUSTFLOAT* outChannel = outBuffer[chan];
                for (int frame = 0; frame < count; frame++) {
                    outChannel[frame] += mixChannel[frame];
                }
            }
        }
    
        // Copy the audio from one buffer to another
        void copy(int count, FAUSTFLOAT** mixBuffer, FAUSTFLOAT** outBuffer)
        {
            for (int chan = 0; chan < getNumOutputs(); chan++) {
                memcpy(outBuffer[chan], mixBuffer[chan], count * sizeof(FAUSTFLOAT));
            }
        }

        // Clear the audio buffer
        void clear(int count, FAUSTFLOAT** outBuffer)
        {
            for (int chan = 0; chan < getNumOutputs(); chan++) {
                memset(outBuffer[chan], 0, count * sizeof(FAUSTFLOAT));
            }
        }
    
        // Get the index of a voice currently playing a specific pitch
        int getPlayingVoice(int pitch)
        {
            int voice_playing = kNoVoice;
            int oldest_date_playing = INT_MAX;
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                int curNote = fVoiceTable[i]->fCurNote;
                int nextNote = fVoiceTable[i]->fNextNote;
                if ((curNote == pitch) || ((curNote == kLegatoVoice) && (nextNote == pitch))) {
                    // Keeps oldest playing voice
                    if (fVoiceTable[i]->fDate < oldest_date_playing) {
                        oldest_date_playing = fVoiceTable[i]->fDate;
                        voice_playing = int(i);
                    }
                }
            }
            
            return voice_playing;
        }
    
        // Allocate a voice with a given type
        int allocVoice(int voice, int type)
        {
            fVoiceTable[voice]->fDate++;
            fVoiceTable[voice]->fCurNote = type;
            return voice;
        }
    
        // Get a free voice for allocation, always returns a voice
        int getFreeVoice()
        {
            // Looks for the first available voice
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                if (fVoiceTable[i]->fCurNote == kFreeVoice) {
                    return allocVoice(int(i), kActiveVoice);
                }
            }

            // Otherwise steal one
            int voice_release = kNoVoice;
            int voice_playing = kNoVoice;
            int oldest_date_release = INT_MAX;
            int oldest_date_playing = INT_MAX;
        
            // Scan all voices
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                if (fVoiceTable[i]->fCurNote == kReleaseVoice) {
                    // Keeps oldest release voice
                    if (fVoiceTable[i]->fDate < oldest_date_release) {
                        oldest_date_release = fVoiceTable[i]->fDate;
                        voice_release = int(i);
                    }
                } else {
                    // Otherwise keeps oldest playing voice
                    if (fVoiceTable[i]->fDate < oldest_date_playing) {
                        oldest_date_playing = fVoiceTable[i]->fDate;
                        voice_playing = int(i);
                    }
                }
            }
        
            
            // Then decide which one to steal
            if (oldest_date_release != INT_MAX) {
                fprintf(stderr, "Steal release voice : voice_date = %d cur_date = %d voice = %d \n",
                        fVoiceTable[voice_release]->fDate,
                        fDate,
                        voice_release);
                return allocVoice(voice_release, kLegatoVoice);
            } else if (oldest_date_playing != INT_MAX) {
                fprintf(stderr, "Steal playing voice : voice_date = %d cur_date = %d voice = %d \n",
                        fVoiceTable[voice_playing]->fDate,
                        fDate,
                        voice_release);
                return allocVoice(voice_playing, kLegatoVoice);
            } else {
                assert(false);
                return kNoVoice;
            }
        }

        // Callback for panic button
        static void panic(FAUSTFLOAT val, void* arg)
        {
            if (val == FAUSTFLOAT(1)) {
                static_cast<mydsp_poly*>(arg)->allNotesOff(true);
            }
        }

        // Check if the DSP is polyphonic
        bool checkPolyphony()
        {
            if (fVoiceTable.size() > 0) {
                return true;
            } else {
                fprintf(stderr, "DSP is not polyphonic...\n");
                return false;
            }
        }

    public:
    
        /**
         * Constructor.
         *
         * @param dsp - the dsp to be used for one voice. Beware: mydsp_poly will use and finally delete the pointer.
         * @param nvoices - number of polyphony voices, should be at least 1
         * @param control - whether voices will be dynamically allocated and controlled (typically by a MIDI controler).
         *                If false all voices are always running.
         * @param group - if true, voices are not individually accessible, a global "Voices" tab will automatically dispatch
         *                a given control on all voices, assuming GUI::updateAllGuis() is called.
         *                If false, all voices can be individually controlled.
         *
         */
        mydsp_poly(::dsp* dsp,
                   int nvoices,
                   bool control = false,
                   bool group = true)
        : dsp_voice_group(panic, this, control, group), dsp_poly(dsp) // dsp parameter is deallocated by ~dsp_poly
        {
            fDate = 0;
            fMidiHandler = nullptr;

            // Create voices
            assert(nvoices > 0);
            for (int i = 0; i < nvoices; i++) {
                addVoice(new dsp_voice(dsp->clone()));
            }

            // Init audio output buffers
            fMixBuffer = new FAUSTFLOAT*[getNumOutputs()];
            fOutBuffer = new FAUSTFLOAT*[getNumOutputs()];
            for (int chan = 0; chan < getNumOutputs(); chan++) {
                fMixBuffer[chan] = new FAUSTFLOAT[MIX_BUFFER_SIZE];
                fOutBuffer[chan] = new FAUSTFLOAT[MIX_BUFFER_SIZE];
            }

            dsp_voice_group::init();
        }

        virtual ~mydsp_poly()
        {
            // Remove from fMidiHandler
            if (fMidiHandler) fMidiHandler->removeMidiIn(this);
            for (int chan = 0; chan < getNumOutputs(); chan++) {
                delete[] fMixBuffer[chan];
                delete[] fOutBuffer[chan];
            }
            delete[] fMixBuffer;
            delete[] fOutBuffer;
            
        }

        // DSP API
        void buildUserInterface(UI* ui_interface)
        {
            // MidiUI ui_interface contains the midi_handler connected to the MIDI driver
            #ifdef DAISY_NO_RTTI
            if (ui_interface->isMidiInterface()) {
                fMidiHandler = reinterpret_cast<midi_interface*>(ui_interface);
                fMidiHandler->addMidiIn(this);
            }
            #else
            if (dynamic_cast<midi_interface*>(ui_interface)) {
                fMidiHandler = dynamic_cast<midi_interface*>(ui_interface);
                fMidiHandler->addMidiIn(this);
            }
            #endif
            dsp_voice_group::buildUserInterface(ui_interface);
        }

        void init(int sample_rate)
        {
            decorator_dsp::init(sample_rate);
            fVoiceGroup->init(sample_rate);
            fPanic = FAUSTFLOAT(0);
            
            // Init voices
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->init(sample_rate);
            }
        }
    
        void instanceInit(int samplingFreq)
        {
            instanceConstants(samplingFreq);
            instanceResetUserInterface();
            instanceClear();
        }

        void instanceConstants(int sample_rate)
        {
            decorator_dsp::instanceConstants(sample_rate);
            fVoiceGroup->instanceConstants(sample_rate);
            
            // Init voices
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceConstants(sample_rate);
            }
        }

        void instanceResetUserInterface()
        {
            decorator_dsp::instanceResetUserInterface();
            fVoiceGroup->instanceResetUserInterface();
            fPanic = FAUSTFLOAT(0);
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceResetUserInterface();
            }
        }

        void instanceClear()
        {
            decorator_dsp::instanceClear();
            fVoiceGroup->instanceClear();
            
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->instanceClear();
            }
        }

        virtual mydsp_poly* clone()
        {
            return new mydsp_poly(fDSP->clone(), int(fVoiceTable.size()), fVoiceControl, fGroupControl);
        }

        void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            assert(count <= MIX_BUFFER_SIZE);

            // First clear the intermediate fOutBuffer
            clear(count, fOutBuffer);

            if (fVoiceControl) {
                // Mix all playing voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    dsp_voice* voice = fVoiceTable[i];
                    if (voice->fCurNote == kLegatoVoice) {
                        // Play from current note and next note
                        voice->computeLegato(count, inputs, fMixBuffer);
                        // FadeOut on first half buffer
                        fadeOut(count/2, fMixBuffer);
                        // Mix it in result
                        voice->fLevel = mixCheckVoice(count, fMixBuffer, fOutBuffer);
                    } else if (voice->fCurNote != kFreeVoice) {
                        // Compute current note
                        voice->compute(count, inputs, fMixBuffer);
                        // Mix it in result
                        voice->fLevel = mixCheckVoice(count, fMixBuffer, fOutBuffer);
                        if ((voice->fCurNote == kReleaseVoice) && (voice->fLevel < VOICE_STOP_LEVEL)) {
                            voice->fCurNote = kFreeVoice;
                        }
                    }
                }
            } else {
                // Mix all voices
                for (size_t i = 0; i < fVoiceTable.size(); i++) {
                    fVoiceTable[i]->compute(count, inputs, fMixBuffer);
                    mixVoice(count, fMixBuffer, fOutBuffer);
                }
            }
            
            // Finally copy intermediate buffer to outputs
            copy(count, fOutBuffer, outputs);
        }

        void compute(double date_usec, int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs)
        {
            compute(count, inputs, outputs);
        }
    
        // Terminate all active voices, gently or immediately (depending of 'hard' value)
        void allNotesOff(bool hard = false)
        {
            for (size_t i = 0; i < fVoiceTable.size(); i++) {
                fVoiceTable[i]->keyOff(hard);
            }
        }
 
        // Additional polyphonic API
        MapUI* newVoice()
        {
            return fVoiceTable[getFreeVoice()];
        }

        void deleteVoice(MapUI* voice)
        {
            auto it = find(fVoiceTable.begin(), fVoiceTable.end(), reinterpret_cast<dsp_voice*>(voice));
            if (it != fVoiceTable.end()) {
                dsp_voice* voice = *it;
                voice->keyOff();
                voice->reset();
            } else {
                fprintf(stderr, "Voice not found\n");
            }
        }

        // MIDI API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            if (checkPolyphony()) {
                int voice = getFreeVoice();
                fVoiceTable[voice]->keyOn(pitch, velocity, fVoiceTable[voice]->fCurNote == kLegatoVoice);
                return fVoiceTable[voice];
            } else {
                return 0;
            }
        }

        void keyOff(int channel, int pitch, int velocity = 127)
        {
            if (checkPolyphony()) {
                int voice = getPlayingVoice(pitch);
                if (voice != kNoVoice) {
                    fVoiceTable[voice]->keyOff();
                } else {
                    fprintf(stderr, "Playing pitch = %d not found\n", pitch);
                }
            }
        }

        void ctrlChange(int channel, int ctrl, int value)
        {
            if (ctrl == ALL_NOTES_OFF || ctrl == ALL_SOUND_OFF) {
                allNotesOff();
            }
        }

};

/**
 * Polyphonic DSP with an integrated effect.
 */
class dsp_poly_effect : public dsp_poly {
    
    private:
    
        // fPolyDSP will respond to MIDI messages.
        dsp_poly* fPolyDSP;
        
    public:
        
        dsp_poly_effect(dsp_poly* voice, ::dsp* combined)
        :dsp_poly(combined), fPolyDSP(voice)
        {}
        
        virtual ~dsp_poly_effect()
        {
            // dsp_poly_effect is also a decorator_dsp, which will free fPolyDSP
        }
    
        // MIDI API
        MapUI* keyOn(int channel, int pitch, int velocity)
        {
            return fPolyDSP->keyOn(channel, pitch, velocity);
        }
        void keyOff(int channel, int pitch, int velocity)
        {
            fPolyDSP->keyOff(channel, pitch, velocity);
        }
        void keyPress(int channel, int pitch, int press)
        {
            fPolyDSP->keyPress(channel, pitch, press);
        }
        void chanPress(int channel, int press)
        {
            fPolyDSP->chanPress(channel, press);
        }
        void ctrlChange(int channel, int ctrl, int value)
        {
            fPolyDSP->ctrlChange(channel, ctrl, value);
        }
        void ctrlChange14bits(int channel, int ctrl, int value)
        {
            fPolyDSP->ctrlChange14bits(channel, ctrl, value);
        }
        void pitchWheel(int channel, int wheel)
        {
            fPolyDSP->pitchWheel(channel, wheel);
        }
        void progChange(int channel, int pgm)
        {
            fPolyDSP->progChange(channel, pgm);
        }
      
};

/**
 * Polyphonic DSP factory class. Helper code to support polyphonic DSP source with an integrated effect.
 */
struct dsp_poly_factory : public dsp_factory {
    
    dsp_factory* fProcessFactory;
    dsp_factory* fEffectFactory;
    
    ::dsp* adaptDSP(::dsp* dsp, bool is_double)
    {
        return (is_double) ? new dsp_sample_adapter<double, float>(dsp) : dsp;
    }

    dsp_poly_factory(dsp_factory* process_factory = nullptr,
                     dsp_factory* effect_factory = nullptr):
    fProcessFactory(process_factory)
    ,fEffectFactory(effect_factory)
    {}

    virtual ~dsp_poly_factory()
    {}

    std::string getName() { return fProcessFactory->getName(); }
    std::string getSHAKey() { return fProcessFactory->getSHAKey(); }
    std::string getDSPCode() { return fProcessFactory->getDSPCode(); }
    std::string getCompileOptions() { return fProcessFactory->getCompileOptions(); }
    std::vector<std::string> getLibraryList() { return fProcessFactory->getLibraryList(); }
    std::vector<std::string> getIncludePathnames() { return fProcessFactory->getIncludePathnames(); }
    std::vector<std::string> getWarningMessages() { return fProcessFactory->getWarningMessages(); }
    std::string getJSON() { return fProcessFactory->getJSON(); }
   
    std::string getEffectCode(const std::string& dsp_content)
    {
        std::stringstream effect_code;
        effect_code << "adapt(1,1) = _; adapt(2,2) = _,_; adapt(1,2) = _ <: _,_; adapt(2,1) = _,_ :> _;";
        effect_code << "adaptor(F,G) = adapt(outputs(F),inputs(G)); dsp_code = environment{ " << dsp_content << " };";
        effect_code << "process = adaptor(dsp_code.process, dsp_code.effect) : dsp_code.effect;";
        return effect_code.str();
    }

    virtual void setMemoryManager(dsp_memory_manager* manager)
    {
        fProcessFactory->setMemoryManager(manager);
        if (fEffectFactory) {
            fEffectFactory->setMemoryManager(manager);
        }
    }
    virtual dsp_memory_manager* getMemoryManager() { return fProcessFactory->getMemoryManager(); }

    /* Create a new polyphonic DSP instance with global effect, to be deleted with C++ 'delete'
     *
     * @param nvoices - number of polyphony voices, should be at least 1.
     * If -1 is used, the voice number found in the 'declare options "[nvoices:N]";' section will be used.
     * @param control - whether voices will be dynamically allocated and controlled (typically by a MIDI controler).
     *                If false all voices are always running.
     * @param group - if true, voices are not individually accessible, a global "Voices" tab will automatically dispatch
     *                a given control on all voices, assuming GUI::updateAllGuis() is called.
     *                If false, all voices can be individually controlled.
     * @param is_double - if true, internally allocated DSPs will be adapted to receive 'double' samples.
     */
    dsp_poly* createPolyDSPInstance(int nvoices, bool control, bool group, bool is_double = false)
    {
        if (nvoices == -1) {
            // Get 'nvoices' from the metadata declaration
            ::dsp* dsp = fProcessFactory->createDSPInstance();
            bool midi_sync = false;
            bool midi = false;
            MidiMeta::analyse(dsp, midi, midi_sync, nvoices);
            delete dsp;
        }
        dsp_poly* dsp_poly = new mydsp_poly(adaptDSP(fProcessFactory->createDSPInstance(), is_double), nvoices, control, group);
        if (fEffectFactory) {
            // the 'dsp_poly' object has to be controlled with MIDI, so kept separated from new dsp_sequencer(...) object
            return new dsp_poly_effect(dsp_poly, new dsp_sequencer(dsp_poly, adaptDSP(fEffectFactory->createDSPInstance(), is_double)));
        } else {
            return new dsp_poly_effect(dsp_poly, dsp_poly);
        }
    }

    /* Create a new DSP instance, to be deleted with C++ 'delete' */
    ::dsp* createDSPInstance()
    {
        return fProcessFactory->createDSPInstance();
    }

};

#endif // __poly_dsp__
/************************** END poly-dsp.h **************************/
#endif

// we require macro declarations
#define FAUST_UIMACROS

// but we will ignore most of them
#define FAUST_ADDBUTTON(l,f)
#define FAUST_ADDCHECKBOX(l,f)
#define FAUST_ADDVERTICALSLIDER(l,f,i,a,b,s)
#define FAUST_ADDHORIZONTALSLIDER(l,f,i,a,b,s)
#define FAUST_ADDNUMENTRY(l,f,i,a,b,s)
#define FAUST_ADDVERTICALBARGRAPH(l,f,a,b)
#define FAUST_ADDHORIZONTALBARGRAPH(l,f,a,b)

/******************************************************************************
 *******************************************************************************

 VECTOR INTRINSICS

 *******************************************************************************
 *******************************************************************************/


/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

#ifndef FAUSTCLASS 
#define FAUSTCLASS mydsp
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

static float mydsp_faustpower2_f(float value) {
	return value * value;
}

struct mydsp : public dsp {
	
	FAUSTFLOAT fHslider0;
	FAUSTFLOAT fHslider1;
	FAUSTFLOAT fHslider2;
	FAUSTFLOAT fHslider3;
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fRec0[3];
	int fVec0_widx;
	float fVec0[3029];
	float fConst3;
	FAUSTFLOAT fHslider4;
	FAUSTFLOAT fHslider5;
	float fRec1[3];
	int fVec1_widx;
	float fVec1[3029];
	FAUSTFLOAT fHslider6;
	FAUSTFLOAT fHslider7;
	float fRec2[3];
	int fVec2_widx;
	float fVec2[3029];
	FAUSTFLOAT fHslider8;
	FAUSTFLOAT fHslider9;
	float fRec3[3];
	int fVec3_widx;
	float fVec3[3029];
	FAUSTFLOAT fHslider10;
	FAUSTFLOAT fHslider11;
	float fRec4[3];
	int fVec4_widx;
	float fVec4[3029];
	FAUSTFLOAT fHslider12;
	FAUSTFLOAT fHslider13;
	float fRec5[3];
	int fVec5_widx;
	float fVec5[3029];
	FAUSTFLOAT fHslider14;
	FAUSTFLOAT fHslider15;
	float fRec6[3];
	int fVec6_widx;
	float fVec6[3029];
	FAUSTFLOAT fHslider16;
	FAUSTFLOAT fHslider17;
	float fRec7[3];
	int fVec7_widx;
	float fVec7[3029];
	FAUSTFLOAT fHslider18;
	FAUSTFLOAT fHslider19;
	float fRec8[3];
	int fVec8_widx;
	float fVec8[3029];
	FAUSTFLOAT fHslider20;
	FAUSTFLOAT fHslider21;
	float fRec9[3];
	int fVec9_widx;
	float fVec9[3029];
	FAUSTFLOAT fHslider22;
	FAUSTFLOAT fHslider23;
	float fRec10[3];
	int fVec10_widx;
	float fVec10[3029];
	FAUSTFLOAT fHslider24;
	FAUSTFLOAT fHslider25;
	float fRec11[3];
	int fVec11_widx;
	float fVec11[3029];
	FAUSTFLOAT fHslider26;
	FAUSTFLOAT fHslider27;
	float fRec12[3];
	int fVec12_widx;
	float fVec12[3029];
	FAUSTFLOAT fHslider28;
	FAUSTFLOAT fHslider29;
	float fRec13[3];
	int fVec13_widx;
	float fVec13[3029];
	FAUSTFLOAT fHslider30;
	FAUSTFLOAT fHslider31;
	float fRec14[3];
	int fVec14_widx;
	float fVec14[3029];
	FAUSTFLOAT fHslider32;
	FAUSTFLOAT fHslider33;
	float fRec15[3];
	int fVec15_widx;
	float fVec15[3029];
	float fRec16[3];
	int fVec16_widx;
	float fVec16[3029];
	float fRec17[3];
	int fVec17_widx;
	float fVec17[3029];
	float fRec18[3];
	int fVec18_widx;
	float fVec18[3029];
	float fRec19[3];
	int fVec19_widx;
	float fVec19[3029];
	float fRec20[3];
	int fVec20_widx;
	float fVec20[3029];
	float fRec21[3];
	int fVec21_widx;
	float fVec21[3029];
	float fRec22[3];
	int fVec22_widx;
	float fVec22[3029];
	float fRec23[3];
	int fVec23_widx;
	float fVec23[3029];
	float fRec24[3];
	int fVec24_widx;
	float fVec24[3029];
	float fRec25[3];
	int fVec25_widx;
	float fVec25[3029];
	float fRec26[3];
	int fVec26_widx;
	float fVec26[3029];
	float fRec27[3];
	int fVec27_widx;
	float fVec27[3029];
	float fRec28[3];
	int fVec28_widx;
	float fVec28[3029];
	float fRec29[3];
	int fVec29_widx;
	float fVec29[3029];
	float fRec30[3];
	int fVec30_widx;
	float fVec30[3029];
	float fRec31[3];
	int fVec31_widx;
	float fVec31[3029];
	
	mydsp() {
	}
	
	mydsp(const mydsp&) = default;
	
	virtual ~mydsp() = default;
	
	mydsp& operator=(const mydsp&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.22.0");
		m->declare("compile_options", "-a ananas.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -dtl 1024 -es 1 -mcd 16 -mdd 1024 -mdy 33 -uim -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("description", "Basic WFS for a distributed setup consisting of modules that each handle two output channels.");
		m->declare("filename", "wfs.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
		m->declare("filters.lib/version", "1.7.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LGPL with exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "Distributed WFS");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
	}

	virtual int getNumInputs() {
		return 16;
	}
	virtual int getNumOutputs() {
		return 2;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 0.002915452f * fConst0;
		fConst2 = 3.1415927f / fConst0;
		fVec0_widx = 0;
		fConst3 = 0.015767718f * fConst0 + 1.0f;
		fVec1_widx = 0;
		fVec2_widx = 0;
		fVec3_widx = 0;
		fVec4_widx = 0;
		fVec5_widx = 0;
		fVec6_widx = 0;
		fVec7_widx = 0;
		fVec8_widx = 0;
		fVec9_widx = 0;
		fVec10_widx = 0;
		fVec11_widx = 0;
		fVec12_widx = 0;
		fVec13_widx = 0;
		fVec14_widx = 0;
		fVec15_widx = 0;
		fVec16_widx = 0;
		fVec17_widx = 0;
		fVec18_widx = 0;
		fVec19_widx = 0;
		fVec20_widx = 0;
		fVec21_widx = 0;
		fVec22_widx = 0;
		fVec23_widx = 0;
		fVec24_widx = 0;
		fVec25_widx = 0;
		fVec26_widx = 0;
		fVec27_widx = 0;
		fVec28_widx = 0;
		fVec29_widx = 0;
		fVec30_widx = 0;
		fVec31_widx = 0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(-1.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.2f);
		fHslider4 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider6 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider7 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider8 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider9 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider10 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider11 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider12 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider13 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider14 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider15 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider16 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider17 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider18 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider19 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider20 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider21 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider22 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider23 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider24 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider25 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider26 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider27 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider28 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider29 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider30 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider31 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider32 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider33 = static_cast<FAUSTFLOAT>(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 3; l0 = l0 + 1) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 3029; l1 = l1 + 1) {
			fVec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = l2 + 1) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3029; l3 = l3 + 1) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = l4 + 1) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3029; l5 = l5 + 1) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = l6 + 1) {
			fRec3[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 3029; l7 = l7 + 1) {
			fVec3[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = l8 + 1) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3029; l9 = l9 + 1) {
			fVec4[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = l10 + 1) {
			fRec5[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3029; l11 = l11 + 1) {
			fVec5[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = l12 + 1) {
			fRec6[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3029; l13 = l13 + 1) {
			fVec6[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = l14 + 1) {
			fRec7[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3029; l15 = l15 + 1) {
			fVec7[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = l16 + 1) {
			fRec8[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3029; l17 = l17 + 1) {
			fVec8[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = l18 + 1) {
			fRec9[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3029; l19 = l19 + 1) {
			fVec9[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = l20 + 1) {
			fRec10[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3029; l21 = l21 + 1) {
			fVec10[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = l22 + 1) {
			fRec11[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3029; l23 = l23 + 1) {
			fVec11[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = l24 + 1) {
			fRec12[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3029; l25 = l25 + 1) {
			fVec12[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = l26 + 1) {
			fRec13[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3029; l27 = l27 + 1) {
			fVec13[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 3; l28 = l28 + 1) {
			fRec14[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3029; l29 = l29 + 1) {
			fVec14[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = l30 + 1) {
			fRec15[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 3029; l31 = l31 + 1) {
			fVec15[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 3; l32 = l32 + 1) {
			fRec16[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3029; l33 = l33 + 1) {
			fVec16[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = l34 + 1) {
			fRec17[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3029; l35 = l35 + 1) {
			fVec17[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = l36 + 1) {
			fRec18[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3029; l37 = l37 + 1) {
			fVec18[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = l38 + 1) {
			fRec19[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3029; l39 = l39 + 1) {
			fVec19[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 3; l40 = l40 + 1) {
			fRec20[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3029; l41 = l41 + 1) {
			fVec20[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = l42 + 1) {
			fRec21[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3029; l43 = l43 + 1) {
			fVec21[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = l44 + 1) {
			fRec22[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3029; l45 = l45 + 1) {
			fVec22[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = l46 + 1) {
			fRec23[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3029; l47 = l47 + 1) {
			fVec23[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = l48 + 1) {
			fRec24[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3029; l49 = l49 + 1) {
			fVec24[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = l50 + 1) {
			fRec25[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3029; l51 = l51 + 1) {
			fVec25[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = l52 + 1) {
			fRec26[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3029; l53 = l53 + 1) {
			fVec26[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 3; l54 = l54 + 1) {
			fRec27[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 3029; l55 = l55 + 1) {
			fVec27[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 3; l56 = l56 + 1) {
			fRec28[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 3029; l57 = l57 + 1) {
			fVec28[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 3; l58 = l58 + 1) {
			fRec29[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 3029; l59 = l59 + 1) {
			fVec29[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 3; l60 = l60 + 1) {
			fRec30[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 3029; l61 = l61 + 1) {
			fVec30[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 3; l62 = l62 + 1) {
			fRec31[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 3029; l63 = l63 + 1) {
			fVec31[l63] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual mydsp* clone() {
		return new mydsp(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("Distributed WFS");
		ui_interface->addHorizontalSlider("0/x", &fHslider33, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("0/y", &fHslider32, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("1/x", &fHslider31, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("1/y", &fHslider30, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("10/x", &fHslider13, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("10/y", &fHslider12, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("11/x", &fHslider11, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("11/y", &fHslider10, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("12/x", &fHslider9, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("12/y", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("13/x", &fHslider7, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("13/y", &fHslider6, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("14/x", &fHslider5, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("14/y", &fHslider4, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("15/x", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("15/y", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("2/x", &fHslider29, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("2/y", &fHslider28, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("3/x", &fHslider27, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("3/y", &fHslider26, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("4/x", &fHslider25, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("4/y", &fHslider24, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("5/x", &fHslider23, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("5/y", &fHslider22, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("6/x", &fHslider21, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("6/y", &fHslider20, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("7/x", &fHslider19, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("7/y", &fHslider18, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("8/x", &fHslider17, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("8/y", &fHslider16, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("9/x", &fHslider15, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("9/y", &fHslider14, FAUSTFLOAT(0.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->openVerticalBox("Global settings");
		ui_interface->addHorizontalSlider("moduleID", &fHslider1, FAUSTFLOAT(-1.0f), FAUSTFLOAT(-1.0f), FAUSTFLOAT(7.0f), FAUSTFLOAT(1.0f));
		ui_interface->declare(&fHslider3, "unit", "m");
		ui_interface->addHorizontalSlider("spacing", &fHslider3, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.05f), FAUSTFLOAT(0.3f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {
		FAUSTFLOAT* input0 = inputs[0];
		FAUSTFLOAT* input1 = inputs[1];
		FAUSTFLOAT* input2 = inputs[2];
		FAUSTFLOAT* input3 = inputs[3];
		FAUSTFLOAT* input4 = inputs[4];
		FAUSTFLOAT* input5 = inputs[5];
		FAUSTFLOAT* input6 = inputs[6];
		FAUSTFLOAT* input7 = inputs[7];
		FAUSTFLOAT* input8 = inputs[8];
		FAUSTFLOAT* input9 = inputs[9];
		FAUSTFLOAT* input10 = inputs[10];
		FAUSTFLOAT* input11 = inputs[11];
		FAUSTFLOAT* input12 = inputs[12];
		FAUSTFLOAT* input13 = inputs[13];
		FAUSTFLOAT* input14 = inputs[14];
		FAUSTFLOAT* input15 = inputs[15];
		FAUSTFLOAT* output0 = outputs[0];
		FAUSTFLOAT* output1 = outputs[1];
		float fSlow0 = static_cast<float>(fHslider0);
		float fSlow1 = ((fSlow0 < 0.0f) ? 3.0f * fSlow0 : 1e+01f * fSlow0);
		int iSlow2 = fSlow1 > 0.0f;
		float fSlow3 = mydsp_faustpower2_f(fSlow1);
		float fSlow4 = 2.0f * static_cast<float>(fHslider1);
		float fSlow5 = 7.5f * (static_cast<float>(fHslider2) + 1.0f);
		float fSlow6 = static_cast<float>(fHslider3);
		float fSlow7 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow5 - fSlow4)) + fSlow3);
		float fSlow8 = std::sqrt(mydsp_faustpower2_f(15.0f * fSlow6) + 9.0f);
		float fSlow9 = ((iSlow2) ? fConst1 * (fSlow7 - fSlow1) : fConst1 * (fSlow8 - fSlow7));
		float fSlow10 = std::floor(fSlow9);
		float fSlow11 = fSlow10 + (1.0f - fSlow9);
		float fSlow12 = std::max<float>(std::sqrt(fSlow7), 1.0f);
		float fSlow13 = std::tan(fConst2 * (1.5e+04f / fSlow12 + 5e+03f));
		float fSlow14 = 1.0f / fSlow13;
		float fSlow15 = (fSlow14 + 1.4142135f) / fSlow13 + 1.0f;
		float fSlow16 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow13);
		float fSlow17 = (fSlow14 + -1.4142135f) / fSlow13 + 1.0f;
		int iSlow18 = static_cast<int>(fSlow9);
		int iSlow19 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow18))));
		float fSlow20 = static_cast<float>(fHslider4);
		float fSlow21 = ((fSlow20 < 0.0f) ? 3.0f * fSlow20 : 1e+01f * fSlow20);
		int iSlow22 = fSlow21 > 0.0f;
		float fSlow23 = mydsp_faustpower2_f(fSlow21);
		float fSlow24 = 7.5f * (static_cast<float>(fHslider5) + 1.0f);
		float fSlow25 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow24 - fSlow4)) + fSlow23);
		float fSlow26 = ((iSlow22) ? fConst1 * (fSlow25 - fSlow21) : fConst1 * (fSlow8 - fSlow25));
		float fSlow27 = std::floor(fSlow26);
		float fSlow28 = fSlow27 + (1.0f - fSlow26);
		float fSlow29 = std::max<float>(std::sqrt(fSlow25), 1.0f);
		float fSlow30 = std::tan(fConst2 * (1.5e+04f / fSlow29 + 5e+03f));
		float fSlow31 = 1.0f / fSlow30;
		float fSlow32 = (fSlow31 + 1.4142135f) / fSlow30 + 1.0f;
		float fSlow33 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow30);
		float fSlow34 = (fSlow31 + -1.4142135f) / fSlow30 + 1.0f;
		int iSlow35 = static_cast<int>(fSlow26);
		int iSlow36 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow35))));
		float fSlow37 = static_cast<float>(fHslider6);
		float fSlow38 = ((fSlow37 < 0.0f) ? 3.0f * fSlow37 : 1e+01f * fSlow37);
		int iSlow39 = fSlow38 > 0.0f;
		float fSlow40 = mydsp_faustpower2_f(fSlow38);
		float fSlow41 = 7.5f * (static_cast<float>(fHslider7) + 1.0f);
		float fSlow42 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow41 - fSlow4)) + fSlow40);
		float fSlow43 = ((iSlow39) ? fConst1 * (fSlow42 - fSlow38) : fConst1 * (fSlow8 - fSlow42));
		float fSlow44 = std::floor(fSlow43);
		float fSlow45 = fSlow44 + (1.0f - fSlow43);
		float fSlow46 = std::max<float>(std::sqrt(fSlow42), 1.0f);
		float fSlow47 = std::tan(fConst2 * (1.5e+04f / fSlow46 + 5e+03f));
		float fSlow48 = 1.0f / fSlow47;
		float fSlow49 = (fSlow48 + 1.4142135f) / fSlow47 + 1.0f;
		float fSlow50 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow47);
		float fSlow51 = (fSlow48 + -1.4142135f) / fSlow47 + 1.0f;
		int iSlow52 = static_cast<int>(fSlow43);
		int iSlow53 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow52))));
		float fSlow54 = static_cast<float>(fHslider8);
		float fSlow55 = ((fSlow54 < 0.0f) ? 3.0f * fSlow54 : 1e+01f * fSlow54);
		int iSlow56 = fSlow55 > 0.0f;
		float fSlow57 = mydsp_faustpower2_f(fSlow55);
		float fSlow58 = 7.5f * (static_cast<float>(fHslider9) + 1.0f);
		float fSlow59 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow58 - fSlow4)) + fSlow57);
		float fSlow60 = ((iSlow56) ? fConst1 * (fSlow59 - fSlow55) : fConst1 * (fSlow8 - fSlow59));
		float fSlow61 = std::floor(fSlow60);
		float fSlow62 = fSlow61 + (1.0f - fSlow60);
		float fSlow63 = std::max<float>(std::sqrt(fSlow59), 1.0f);
		float fSlow64 = std::tan(fConst2 * (1.5e+04f / fSlow63 + 5e+03f));
		float fSlow65 = 1.0f / fSlow64;
		float fSlow66 = (fSlow65 + 1.4142135f) / fSlow64 + 1.0f;
		float fSlow67 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow64);
		float fSlow68 = (fSlow65 + -1.4142135f) / fSlow64 + 1.0f;
		int iSlow69 = static_cast<int>(fSlow60);
		int iSlow70 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow69))));
		float fSlow71 = static_cast<float>(fHslider10);
		float fSlow72 = ((fSlow71 < 0.0f) ? 3.0f * fSlow71 : 1e+01f * fSlow71);
		int iSlow73 = fSlow72 > 0.0f;
		float fSlow74 = mydsp_faustpower2_f(fSlow72);
		float fSlow75 = 7.5f * (static_cast<float>(fHslider11) + 1.0f);
		float fSlow76 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow75 - fSlow4)) + fSlow74);
		float fSlow77 = ((iSlow73) ? fConst1 * (fSlow76 - fSlow72) : fConst1 * (fSlow8 - fSlow76));
		float fSlow78 = std::floor(fSlow77);
		float fSlow79 = fSlow78 + (1.0f - fSlow77);
		float fSlow80 = std::max<float>(std::sqrt(fSlow76), 1.0f);
		float fSlow81 = std::tan(fConst2 * (1.5e+04f / fSlow80 + 5e+03f));
		float fSlow82 = 1.0f / fSlow81;
		float fSlow83 = (fSlow82 + 1.4142135f) / fSlow81 + 1.0f;
		float fSlow84 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow81);
		float fSlow85 = (fSlow82 + -1.4142135f) / fSlow81 + 1.0f;
		int iSlow86 = static_cast<int>(fSlow77);
		int iSlow87 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow86))));
		float fSlow88 = static_cast<float>(fHslider12);
		float fSlow89 = ((fSlow88 < 0.0f) ? 3.0f * fSlow88 : 1e+01f * fSlow88);
		int iSlow90 = fSlow89 > 0.0f;
		float fSlow91 = mydsp_faustpower2_f(fSlow89);
		float fSlow92 = 7.5f * (static_cast<float>(fHslider13) + 1.0f);
		float fSlow93 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow92 - fSlow4)) + fSlow91);
		float fSlow94 = ((iSlow90) ? fConst1 * (fSlow93 - fSlow89) : fConst1 * (fSlow8 - fSlow93));
		float fSlow95 = std::floor(fSlow94);
		float fSlow96 = fSlow95 + (1.0f - fSlow94);
		float fSlow97 = std::max<float>(std::sqrt(fSlow93), 1.0f);
		float fSlow98 = std::tan(fConst2 * (1.5e+04f / fSlow97 + 5e+03f));
		float fSlow99 = 1.0f / fSlow98;
		float fSlow100 = (fSlow99 + 1.4142135f) / fSlow98 + 1.0f;
		float fSlow101 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow98);
		float fSlow102 = (fSlow99 + -1.4142135f) / fSlow98 + 1.0f;
		int iSlow103 = static_cast<int>(fSlow94);
		int iSlow104 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow103))));
		float fSlow105 = static_cast<float>(fHslider14);
		float fSlow106 = ((fSlow105 < 0.0f) ? 3.0f * fSlow105 : 1e+01f * fSlow105);
		int iSlow107 = fSlow106 > 0.0f;
		float fSlow108 = mydsp_faustpower2_f(fSlow106);
		float fSlow109 = 7.5f * (static_cast<float>(fHslider15) + 1.0f);
		float fSlow110 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow109 - fSlow4)) + fSlow108);
		float fSlow111 = ((iSlow107) ? fConst1 * (fSlow110 - fSlow106) : fConst1 * (fSlow8 - fSlow110));
		float fSlow112 = std::floor(fSlow111);
		float fSlow113 = fSlow112 + (1.0f - fSlow111);
		float fSlow114 = std::max<float>(std::sqrt(fSlow110), 1.0f);
		float fSlow115 = std::tan(fConst2 * (1.5e+04f / fSlow114 + 5e+03f));
		float fSlow116 = 1.0f / fSlow115;
		float fSlow117 = (fSlow116 + 1.4142135f) / fSlow115 + 1.0f;
		float fSlow118 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow115);
		float fSlow119 = (fSlow116 + -1.4142135f) / fSlow115 + 1.0f;
		int iSlow120 = static_cast<int>(fSlow111);
		int iSlow121 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow120))));
		float fSlow122 = static_cast<float>(fHslider16);
		float fSlow123 = ((fSlow122 < 0.0f) ? 3.0f * fSlow122 : 1e+01f * fSlow122);
		int iSlow124 = fSlow123 > 0.0f;
		float fSlow125 = mydsp_faustpower2_f(fSlow123);
		float fSlow126 = 7.5f * (static_cast<float>(fHslider17) + 1.0f);
		float fSlow127 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow126 - fSlow4)) + fSlow125);
		float fSlow128 = ((iSlow124) ? fConst1 * (fSlow127 - fSlow123) : fConst1 * (fSlow8 - fSlow127));
		float fSlow129 = std::floor(fSlow128);
		float fSlow130 = fSlow129 + (1.0f - fSlow128);
		float fSlow131 = std::max<float>(std::sqrt(fSlow127), 1.0f);
		float fSlow132 = std::tan(fConst2 * (1.5e+04f / fSlow131 + 5e+03f));
		float fSlow133 = 1.0f / fSlow132;
		float fSlow134 = (fSlow133 + 1.4142135f) / fSlow132 + 1.0f;
		float fSlow135 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow132);
		float fSlow136 = (fSlow133 + -1.4142135f) / fSlow132 + 1.0f;
		int iSlow137 = static_cast<int>(fSlow128);
		int iSlow138 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow137))));
		float fSlow139 = static_cast<float>(fHslider18);
		float fSlow140 = ((fSlow139 < 0.0f) ? 3.0f * fSlow139 : 1e+01f * fSlow139);
		int iSlow141 = fSlow140 > 0.0f;
		float fSlow142 = mydsp_faustpower2_f(fSlow140);
		float fSlow143 = 7.5f * (static_cast<float>(fHslider19) + 1.0f);
		float fSlow144 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow143 - fSlow4)) + fSlow142);
		float fSlow145 = ((iSlow141) ? fConst1 * (fSlow144 - fSlow140) : fConst1 * (fSlow8 - fSlow144));
		float fSlow146 = std::floor(fSlow145);
		float fSlow147 = fSlow146 + (1.0f - fSlow145);
		float fSlow148 = std::max<float>(std::sqrt(fSlow144), 1.0f);
		float fSlow149 = std::tan(fConst2 * (1.5e+04f / fSlow148 + 5e+03f));
		float fSlow150 = 1.0f / fSlow149;
		float fSlow151 = (fSlow150 + 1.4142135f) / fSlow149 + 1.0f;
		float fSlow152 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow149);
		float fSlow153 = (fSlow150 + -1.4142135f) / fSlow149 + 1.0f;
		int iSlow154 = static_cast<int>(fSlow145);
		int iSlow155 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow154))));
		float fSlow156 = static_cast<float>(fHslider20);
		float fSlow157 = ((fSlow156 < 0.0f) ? 3.0f * fSlow156 : 1e+01f * fSlow156);
		int iSlow158 = fSlow157 > 0.0f;
		float fSlow159 = mydsp_faustpower2_f(fSlow157);
		float fSlow160 = 7.5f * (static_cast<float>(fHslider21) + 1.0f);
		float fSlow161 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow160 - fSlow4)) + fSlow159);
		float fSlow162 = ((iSlow158) ? fConst1 * (fSlow161 - fSlow157) : fConst1 * (fSlow8 - fSlow161));
		float fSlow163 = std::floor(fSlow162);
		float fSlow164 = fSlow163 + (1.0f - fSlow162);
		float fSlow165 = std::max<float>(std::sqrt(fSlow161), 1.0f);
		float fSlow166 = std::tan(fConst2 * (1.5e+04f / fSlow165 + 5e+03f));
		float fSlow167 = 1.0f / fSlow166;
		float fSlow168 = (fSlow167 + 1.4142135f) / fSlow166 + 1.0f;
		float fSlow169 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow166);
		float fSlow170 = (fSlow167 + -1.4142135f) / fSlow166 + 1.0f;
		int iSlow171 = static_cast<int>(fSlow162);
		int iSlow172 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow171))));
		float fSlow173 = static_cast<float>(fHslider22);
		float fSlow174 = ((fSlow173 < 0.0f) ? 3.0f * fSlow173 : 1e+01f * fSlow173);
		int iSlow175 = fSlow174 > 0.0f;
		float fSlow176 = mydsp_faustpower2_f(fSlow174);
		float fSlow177 = 7.5f * (static_cast<float>(fHslider23) + 1.0f);
		float fSlow178 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow177 - fSlow4)) + fSlow176);
		float fSlow179 = ((iSlow175) ? fConst1 * (fSlow178 - fSlow174) : fConst1 * (fSlow8 - fSlow178));
		float fSlow180 = std::floor(fSlow179);
		float fSlow181 = fSlow180 + (1.0f - fSlow179);
		float fSlow182 = std::max<float>(std::sqrt(fSlow178), 1.0f);
		float fSlow183 = std::tan(fConst2 * (1.5e+04f / fSlow182 + 5e+03f));
		float fSlow184 = 1.0f / fSlow183;
		float fSlow185 = (fSlow184 + 1.4142135f) / fSlow183 + 1.0f;
		float fSlow186 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow183);
		float fSlow187 = (fSlow184 + -1.4142135f) / fSlow183 + 1.0f;
		int iSlow188 = static_cast<int>(fSlow179);
		int iSlow189 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow188))));
		float fSlow190 = static_cast<float>(fHslider24);
		float fSlow191 = ((fSlow190 < 0.0f) ? 3.0f * fSlow190 : 1e+01f * fSlow190);
		int iSlow192 = fSlow191 > 0.0f;
		float fSlow193 = mydsp_faustpower2_f(fSlow191);
		float fSlow194 = 7.5f * (static_cast<float>(fHslider25) + 1.0f);
		float fSlow195 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow194 - fSlow4)) + fSlow193);
		float fSlow196 = ((iSlow192) ? fConst1 * (fSlow195 - fSlow191) : fConst1 * (fSlow8 - fSlow195));
		float fSlow197 = std::floor(fSlow196);
		float fSlow198 = fSlow197 + (1.0f - fSlow196);
		float fSlow199 = std::max<float>(std::sqrt(fSlow195), 1.0f);
		float fSlow200 = std::tan(fConst2 * (1.5e+04f / fSlow199 + 5e+03f));
		float fSlow201 = 1.0f / fSlow200;
		float fSlow202 = (fSlow201 + 1.4142135f) / fSlow200 + 1.0f;
		float fSlow203 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow200);
		float fSlow204 = (fSlow201 + -1.4142135f) / fSlow200 + 1.0f;
		int iSlow205 = static_cast<int>(fSlow196);
		int iSlow206 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow205))));
		float fSlow207 = static_cast<float>(fHslider26);
		float fSlow208 = ((fSlow207 < 0.0f) ? 3.0f * fSlow207 : 1e+01f * fSlow207);
		int iSlow209 = fSlow208 > 0.0f;
		float fSlow210 = mydsp_faustpower2_f(fSlow208);
		float fSlow211 = 7.5f * (static_cast<float>(fHslider27) + 1.0f);
		float fSlow212 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow211 - fSlow4)) + fSlow210);
		float fSlow213 = ((iSlow209) ? fConst1 * (fSlow212 - fSlow208) : fConst1 * (fSlow8 - fSlow212));
		float fSlow214 = std::floor(fSlow213);
		float fSlow215 = fSlow214 + (1.0f - fSlow213);
		float fSlow216 = std::max<float>(std::sqrt(fSlow212), 1.0f);
		float fSlow217 = std::tan(fConst2 * (1.5e+04f / fSlow216 + 5e+03f));
		float fSlow218 = 1.0f / fSlow217;
		float fSlow219 = (fSlow218 + 1.4142135f) / fSlow217 + 1.0f;
		float fSlow220 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow217);
		float fSlow221 = (fSlow218 + -1.4142135f) / fSlow217 + 1.0f;
		int iSlow222 = static_cast<int>(fSlow213);
		int iSlow223 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow222))));
		float fSlow224 = static_cast<float>(fHslider28);
		float fSlow225 = ((fSlow224 < 0.0f) ? 3.0f * fSlow224 : 1e+01f * fSlow224);
		int iSlow226 = fSlow225 > 0.0f;
		float fSlow227 = mydsp_faustpower2_f(fSlow225);
		float fSlow228 = 7.5f * (static_cast<float>(fHslider29) + 1.0f);
		float fSlow229 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow228 - fSlow4)) + fSlow227);
		float fSlow230 = ((iSlow226) ? fConst1 * (fSlow229 - fSlow225) : fConst1 * (fSlow8 - fSlow229));
		float fSlow231 = std::floor(fSlow230);
		float fSlow232 = fSlow231 + (1.0f - fSlow230);
		float fSlow233 = std::max<float>(std::sqrt(fSlow229), 1.0f);
		float fSlow234 = std::tan(fConst2 * (1.5e+04f / fSlow233 + 5e+03f));
		float fSlow235 = 1.0f / fSlow234;
		float fSlow236 = (fSlow235 + 1.4142135f) / fSlow234 + 1.0f;
		float fSlow237 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow234);
		float fSlow238 = (fSlow235 + -1.4142135f) / fSlow234 + 1.0f;
		int iSlow239 = static_cast<int>(fSlow230);
		int iSlow240 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow239))));
		float fSlow241 = static_cast<float>(fHslider30);
		float fSlow242 = ((fSlow241 < 0.0f) ? 3.0f * fSlow241 : 1e+01f * fSlow241);
		int iSlow243 = fSlow242 > 0.0f;
		float fSlow244 = mydsp_faustpower2_f(fSlow242);
		float fSlow245 = 7.5f * (static_cast<float>(fHslider31) + 1.0f);
		float fSlow246 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow245 - fSlow4)) + fSlow244);
		float fSlow247 = ((iSlow243) ? fConst1 * (fSlow246 - fSlow242) : fConst1 * (fSlow8 - fSlow246));
		float fSlow248 = std::floor(fSlow247);
		float fSlow249 = fSlow248 + (1.0f - fSlow247);
		float fSlow250 = std::max<float>(std::sqrt(fSlow246), 1.0f);
		float fSlow251 = std::tan(fConst2 * (1.5e+04f / fSlow250 + 5e+03f));
		float fSlow252 = 1.0f / fSlow251;
		float fSlow253 = (fSlow252 + 1.4142135f) / fSlow251 + 1.0f;
		float fSlow254 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow251);
		float fSlow255 = (fSlow252 + -1.4142135f) / fSlow251 + 1.0f;
		int iSlow256 = static_cast<int>(fSlow247);
		int iSlow257 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow256))));
		float fSlow258 = static_cast<float>(fHslider32);
		float fSlow259 = ((fSlow258 < 0.0f) ? 3.0f * fSlow258 : 1e+01f * fSlow258);
		int iSlow260 = fSlow259 > 0.0f;
		float fSlow261 = mydsp_faustpower2_f(fSlow259);
		float fSlow262 = 7.5f * (static_cast<float>(fHslider33) + 1.0f);
		float fSlow263 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow262 - fSlow4)) + fSlow261);
		float fSlow264 = std::max<float>(std::sqrt(fSlow263), 1.0f);
		float fSlow265 = std::tan(fConst2 * (1.5e+04f / fSlow264 + 5e+03f));
		float fSlow266 = 1.0f / fSlow265;
		float fSlow267 = (fSlow266 + 1.4142135f) / fSlow265 + 1.0f;
		float fSlow268 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow265);
		float fSlow269 = (fSlow266 + -1.4142135f) / fSlow265 + 1.0f;
		float fSlow270 = ((iSlow260) ? fConst1 * (fSlow263 - fSlow259) : fConst1 * (fSlow8 - fSlow263));
		int iSlow271 = static_cast<int>(fSlow270);
		int iSlow272 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow271 + 1))));
		float fSlow273 = std::floor(fSlow270);
		float fSlow274 = fSlow270 - fSlow273;
		float fSlow275 = fSlow273 + (1.0f - fSlow270);
		int iSlow276 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow271))));
		int iSlow277 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow256 + 1))));
		float fSlow278 = fSlow247 - fSlow248;
		int iSlow279 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow239 + 1))));
		float fSlow280 = fSlow230 - fSlow231;
		int iSlow281 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow222 + 1))));
		float fSlow282 = fSlow213 - fSlow214;
		int iSlow283 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow205 + 1))));
		float fSlow284 = fSlow196 - fSlow197;
		int iSlow285 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow188 + 1))));
		float fSlow286 = fSlow179 - fSlow180;
		int iSlow287 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow171 + 1))));
		float fSlow288 = fSlow162 - fSlow163;
		int iSlow289 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow154 + 1))));
		float fSlow290 = fSlow145 - fSlow146;
		int iSlow291 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow137 + 1))));
		float fSlow292 = fSlow128 - fSlow129;
		int iSlow293 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow120 + 1))));
		float fSlow294 = fSlow111 - fSlow112;
		int iSlow295 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow103 + 1))));
		float fSlow296 = fSlow94 - fSlow95;
		int iSlow297 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow86 + 1))));
		float fSlow298 = fSlow77 - fSlow78;
		int iSlow299 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow69 + 1))));
		float fSlow300 = fSlow60 - fSlow61;
		int iSlow301 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow52 + 1))));
		float fSlow302 = fSlow43 - fSlow44;
		int iSlow303 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow35 + 1))));
		float fSlow304 = fSlow26 - fSlow27;
		int iSlow305 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow18 + 1))));
		float fSlow306 = fSlow9 - fSlow10;
		float fSlow307 = fSlow4 + 1.0f;
		float fSlow308 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow5 - fSlow307)) + fSlow3);
		float fSlow309 = ((iSlow2) ? fConst1 * (fSlow308 - fSlow1) : fConst1 * (fSlow8 - fSlow308));
		float fSlow310 = std::floor(fSlow309);
		float fSlow311 = fSlow310 + (1.0f - fSlow309);
		float fSlow312 = std::max<float>(std::sqrt(fSlow308), 1.0f);
		float fSlow313 = std::tan(fConst2 * (1.5e+04f / fSlow312 + 5e+03f));
		float fSlow314 = 1.0f / fSlow313;
		float fSlow315 = (fSlow314 + 1.4142135f) / fSlow313 + 1.0f;
		float fSlow316 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow313);
		float fSlow317 = (fSlow314 + -1.4142135f) / fSlow313 + 1.0f;
		int iSlow318 = static_cast<int>(fSlow309);
		int iSlow319 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow318))));
		float fSlow320 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow24 - fSlow307)) + fSlow23);
		float fSlow321 = ((iSlow22) ? fConst1 * (fSlow320 - fSlow21) : fConst1 * (fSlow8 - fSlow320));
		float fSlow322 = std::floor(fSlow321);
		float fSlow323 = fSlow322 + (1.0f - fSlow321);
		float fSlow324 = std::max<float>(std::sqrt(fSlow320), 1.0f);
		float fSlow325 = std::tan(fConst2 * (1.5e+04f / fSlow324 + 5e+03f));
		float fSlow326 = 1.0f / fSlow325;
		float fSlow327 = (fSlow326 + 1.4142135f) / fSlow325 + 1.0f;
		float fSlow328 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow325);
		float fSlow329 = (fSlow326 + -1.4142135f) / fSlow325 + 1.0f;
		int iSlow330 = static_cast<int>(fSlow321);
		int iSlow331 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow330))));
		float fSlow332 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow41 - fSlow307)) + fSlow40);
		float fSlow333 = ((iSlow39) ? fConst1 * (fSlow332 - fSlow38) : fConst1 * (fSlow8 - fSlow332));
		float fSlow334 = std::floor(fSlow333);
		float fSlow335 = fSlow334 + (1.0f - fSlow333);
		float fSlow336 = std::max<float>(std::sqrt(fSlow332), 1.0f);
		float fSlow337 = std::tan(fConst2 * (1.5e+04f / fSlow336 + 5e+03f));
		float fSlow338 = 1.0f / fSlow337;
		float fSlow339 = (fSlow338 + 1.4142135f) / fSlow337 + 1.0f;
		float fSlow340 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow337);
		float fSlow341 = (fSlow338 + -1.4142135f) / fSlow337 + 1.0f;
		int iSlow342 = static_cast<int>(fSlow333);
		int iSlow343 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow342))));
		float fSlow344 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow58 - fSlow307)) + fSlow57);
		float fSlow345 = ((iSlow56) ? fConst1 * (fSlow344 - fSlow55) : fConst1 * (fSlow8 - fSlow344));
		float fSlow346 = std::floor(fSlow345);
		float fSlow347 = fSlow346 + (1.0f - fSlow345);
		float fSlow348 = std::max<float>(std::sqrt(fSlow344), 1.0f);
		float fSlow349 = std::tan(fConst2 * (1.5e+04f / fSlow348 + 5e+03f));
		float fSlow350 = 1.0f / fSlow349;
		float fSlow351 = (fSlow350 + 1.4142135f) / fSlow349 + 1.0f;
		float fSlow352 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow349);
		float fSlow353 = (fSlow350 + -1.4142135f) / fSlow349 + 1.0f;
		int iSlow354 = static_cast<int>(fSlow345);
		int iSlow355 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow354))));
		float fSlow356 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow75 - fSlow307)) + fSlow74);
		float fSlow357 = ((iSlow73) ? fConst1 * (fSlow356 - fSlow72) : fConst1 * (fSlow8 - fSlow356));
		float fSlow358 = std::floor(fSlow357);
		float fSlow359 = fSlow358 + (1.0f - fSlow357);
		float fSlow360 = std::max<float>(std::sqrt(fSlow356), 1.0f);
		float fSlow361 = std::tan(fConst2 * (1.5e+04f / fSlow360 + 5e+03f));
		float fSlow362 = 1.0f / fSlow361;
		float fSlow363 = (fSlow362 + 1.4142135f) / fSlow361 + 1.0f;
		float fSlow364 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow361);
		float fSlow365 = (fSlow362 + -1.4142135f) / fSlow361 + 1.0f;
		int iSlow366 = static_cast<int>(fSlow357);
		int iSlow367 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow366))));
		float fSlow368 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow92 - fSlow307)) + fSlow91);
		float fSlow369 = ((iSlow90) ? fConst1 * (fSlow368 - fSlow89) : fConst1 * (fSlow8 - fSlow368));
		float fSlow370 = std::floor(fSlow369);
		float fSlow371 = fSlow370 + (1.0f - fSlow369);
		float fSlow372 = std::max<float>(std::sqrt(fSlow368), 1.0f);
		float fSlow373 = std::tan(fConst2 * (1.5e+04f / fSlow372 + 5e+03f));
		float fSlow374 = 1.0f / fSlow373;
		float fSlow375 = (fSlow374 + 1.4142135f) / fSlow373 + 1.0f;
		float fSlow376 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow373);
		float fSlow377 = (fSlow374 + -1.4142135f) / fSlow373 + 1.0f;
		int iSlow378 = static_cast<int>(fSlow369);
		int iSlow379 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow378))));
		float fSlow380 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow109 - fSlow307)) + fSlow108);
		float fSlow381 = ((iSlow107) ? fConst1 * (fSlow380 - fSlow106) : fConst1 * (fSlow8 - fSlow380));
		float fSlow382 = std::floor(fSlow381);
		float fSlow383 = fSlow382 + (1.0f - fSlow381);
		float fSlow384 = std::max<float>(std::sqrt(fSlow380), 1.0f);
		float fSlow385 = std::tan(fConst2 * (1.5e+04f / fSlow384 + 5e+03f));
		float fSlow386 = 1.0f / fSlow385;
		float fSlow387 = (fSlow386 + 1.4142135f) / fSlow385 + 1.0f;
		float fSlow388 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow385);
		float fSlow389 = (fSlow386 + -1.4142135f) / fSlow385 + 1.0f;
		int iSlow390 = static_cast<int>(fSlow381);
		int iSlow391 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow390))));
		float fSlow392 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow126 - fSlow307)) + fSlow125);
		float fSlow393 = ((iSlow124) ? fConst1 * (fSlow392 - fSlow123) : fConst1 * (fSlow8 - fSlow392));
		float fSlow394 = std::floor(fSlow393);
		float fSlow395 = fSlow394 + (1.0f - fSlow393);
		float fSlow396 = std::max<float>(std::sqrt(fSlow392), 1.0f);
		float fSlow397 = std::tan(fConst2 * (1.5e+04f / fSlow396 + 5e+03f));
		float fSlow398 = 1.0f / fSlow397;
		float fSlow399 = (fSlow398 + 1.4142135f) / fSlow397 + 1.0f;
		float fSlow400 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow397);
		float fSlow401 = (fSlow398 + -1.4142135f) / fSlow397 + 1.0f;
		int iSlow402 = static_cast<int>(fSlow393);
		int iSlow403 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow402))));
		float fSlow404 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow143 - fSlow307)) + fSlow142);
		float fSlow405 = ((iSlow141) ? fConst1 * (fSlow404 - fSlow140) : fConst1 * (fSlow8 - fSlow404));
		float fSlow406 = std::floor(fSlow405);
		float fSlow407 = fSlow406 + (1.0f - fSlow405);
		float fSlow408 = std::max<float>(std::sqrt(fSlow404), 1.0f);
		float fSlow409 = std::tan(fConst2 * (1.5e+04f / fSlow408 + 5e+03f));
		float fSlow410 = 1.0f / fSlow409;
		float fSlow411 = (fSlow410 + 1.4142135f) / fSlow409 + 1.0f;
		float fSlow412 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow409);
		float fSlow413 = (fSlow410 + -1.4142135f) / fSlow409 + 1.0f;
		int iSlow414 = static_cast<int>(fSlow405);
		int iSlow415 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow414))));
		float fSlow416 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow160 - fSlow307)) + fSlow159);
		float fSlow417 = ((iSlow158) ? fConst1 * (fSlow416 - fSlow157) : fConst1 * (fSlow8 - fSlow416));
		float fSlow418 = std::floor(fSlow417);
		float fSlow419 = fSlow418 + (1.0f - fSlow417);
		float fSlow420 = std::max<float>(std::sqrt(fSlow416), 1.0f);
		float fSlow421 = std::tan(fConst2 * (1.5e+04f / fSlow420 + 5e+03f));
		float fSlow422 = 1.0f / fSlow421;
		float fSlow423 = (fSlow422 + 1.4142135f) / fSlow421 + 1.0f;
		float fSlow424 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow421);
		float fSlow425 = (fSlow422 + -1.4142135f) / fSlow421 + 1.0f;
		int iSlow426 = static_cast<int>(fSlow417);
		int iSlow427 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow426))));
		float fSlow428 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow177 - fSlow307)) + fSlow176);
		float fSlow429 = ((iSlow175) ? fConst1 * (fSlow428 - fSlow174) : fConst1 * (fSlow8 - fSlow428));
		float fSlow430 = std::floor(fSlow429);
		float fSlow431 = fSlow430 + (1.0f - fSlow429);
		float fSlow432 = std::max<float>(std::sqrt(fSlow428), 1.0f);
		float fSlow433 = std::tan(fConst2 * (1.5e+04f / fSlow432 + 5e+03f));
		float fSlow434 = 1.0f / fSlow433;
		float fSlow435 = (fSlow434 + 1.4142135f) / fSlow433 + 1.0f;
		float fSlow436 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow433);
		float fSlow437 = (fSlow434 + -1.4142135f) / fSlow433 + 1.0f;
		int iSlow438 = static_cast<int>(fSlow429);
		int iSlow439 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow438))));
		float fSlow440 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow194 - fSlow307)) + fSlow193);
		float fSlow441 = ((iSlow192) ? fConst1 * (fSlow440 - fSlow191) : fConst1 * (fSlow8 - fSlow440));
		float fSlow442 = std::floor(fSlow441);
		float fSlow443 = fSlow442 + (1.0f - fSlow441);
		float fSlow444 = std::max<float>(std::sqrt(fSlow440), 1.0f);
		float fSlow445 = std::tan(fConst2 * (1.5e+04f / fSlow444 + 5e+03f));
		float fSlow446 = 1.0f / fSlow445;
		float fSlow447 = (fSlow446 + 1.4142135f) / fSlow445 + 1.0f;
		float fSlow448 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow445);
		float fSlow449 = (fSlow446 + -1.4142135f) / fSlow445 + 1.0f;
		int iSlow450 = static_cast<int>(fSlow441);
		int iSlow451 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow450))));
		float fSlow452 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow211 - fSlow307)) + fSlow210);
		float fSlow453 = ((iSlow209) ? fConst1 * (fSlow452 - fSlow208) : fConst1 * (fSlow8 - fSlow452));
		float fSlow454 = std::floor(fSlow453);
		float fSlow455 = fSlow454 + (1.0f - fSlow453);
		float fSlow456 = std::max<float>(std::sqrt(fSlow452), 1.0f);
		float fSlow457 = std::tan(fConst2 * (1.5e+04f / fSlow456 + 5e+03f));
		float fSlow458 = 1.0f / fSlow457;
		float fSlow459 = (fSlow458 + 1.4142135f) / fSlow457 + 1.0f;
		float fSlow460 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow457);
		float fSlow461 = (fSlow458 + -1.4142135f) / fSlow457 + 1.0f;
		int iSlow462 = static_cast<int>(fSlow453);
		int iSlow463 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow462))));
		float fSlow464 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow228 - fSlow307)) + fSlow227);
		float fSlow465 = ((iSlow226) ? fConst1 * (fSlow464 - fSlow225) : fConst1 * (fSlow8 - fSlow464));
		float fSlow466 = std::floor(fSlow465);
		float fSlow467 = fSlow466 + (1.0f - fSlow465);
		float fSlow468 = std::max<float>(std::sqrt(fSlow464), 1.0f);
		float fSlow469 = std::tan(fConst2 * (1.5e+04f / fSlow468 + 5e+03f));
		float fSlow470 = 1.0f / fSlow469;
		float fSlow471 = (fSlow470 + 1.4142135f) / fSlow469 + 1.0f;
		float fSlow472 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow469);
		float fSlow473 = (fSlow470 + -1.4142135f) / fSlow469 + 1.0f;
		int iSlow474 = static_cast<int>(fSlow465);
		int iSlow475 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow474))));
		float fSlow476 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow245 - fSlow307)) + fSlow244);
		float fSlow477 = ((iSlow243) ? fConst1 * (fSlow476 - fSlow242) : fConst1 * (fSlow8 - fSlow476));
		float fSlow478 = std::floor(fSlow477);
		float fSlow479 = fSlow478 + (1.0f - fSlow477);
		float fSlow480 = std::max<float>(std::sqrt(fSlow476), 1.0f);
		float fSlow481 = std::tan(fConst2 * (1.5e+04f / fSlow480 + 5e+03f));
		float fSlow482 = 1.0f / fSlow481;
		float fSlow483 = (fSlow482 + 1.4142135f) / fSlow481 + 1.0f;
		float fSlow484 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow481);
		float fSlow485 = (fSlow482 + -1.4142135f) / fSlow481 + 1.0f;
		int iSlow486 = static_cast<int>(fSlow477);
		int iSlow487 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow486))));
		float fSlow488 = std::sqrt(mydsp_faustpower2_f(fSlow6 * (fSlow262 - fSlow307)) + fSlow261);
		float fSlow489 = std::max<float>(std::sqrt(fSlow488), 1.0f);
		float fSlow490 = std::tan(fConst2 * (1.5e+04f / fSlow489 + 5e+03f));
		float fSlow491 = 1.0f / fSlow490;
		float fSlow492 = (fSlow491 + 1.4142135f) / fSlow490 + 1.0f;
		float fSlow493 = 1.0f - 1.0f / mydsp_faustpower2_f(fSlow490);
		float fSlow494 = (fSlow491 + -1.4142135f) / fSlow490 + 1.0f;
		float fSlow495 = ((iSlow260) ? fConst1 * (fSlow488 - fSlow259) : fConst1 * (fSlow8 - fSlow488));
		int iSlow496 = static_cast<int>(fSlow495);
		int iSlow497 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow496 + 1))));
		float fSlow498 = std::floor(fSlow495);
		float fSlow499 = fSlow495 - fSlow498;
		float fSlow500 = fSlow498 + (1.0f - fSlow495);
		int iSlow501 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow496))));
		int iSlow502 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow486 + 1))));
		float fSlow503 = fSlow477 - fSlow478;
		int iSlow504 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow474 + 1))));
		float fSlow505 = fSlow465 - fSlow466;
		int iSlow506 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow462 + 1))));
		float fSlow507 = fSlow453 - fSlow454;
		int iSlow508 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow450 + 1))));
		float fSlow509 = fSlow441 - fSlow442;
		int iSlow510 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow438 + 1))));
		float fSlow511 = fSlow429 - fSlow430;
		int iSlow512 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow426 + 1))));
		float fSlow513 = fSlow417 - fSlow418;
		int iSlow514 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow414 + 1))));
		float fSlow515 = fSlow405 - fSlow406;
		int iSlow516 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow402 + 1))));
		float fSlow517 = fSlow393 - fSlow394;
		int iSlow518 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow390 + 1))));
		float fSlow519 = fSlow381 - fSlow382;
		int iSlow520 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow378 + 1))));
		float fSlow521 = fSlow369 - fSlow370;
		int iSlow522 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow366 + 1))));
		float fSlow523 = fSlow357 - fSlow358;
		int iSlow524 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow354 + 1))));
		float fSlow525 = fSlow345 - fSlow346;
		int iSlow526 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow342 + 1))));
		float fSlow527 = fSlow333 - fSlow334;
		int iSlow528 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow330 + 1))));
		float fSlow529 = fSlow321 - fSlow322;
		int iSlow530 = static_cast<int>(std::min<float>(fConst3, static_cast<float>(std::max<int>(0, iSlow318 + 1))));
		float fSlow531 = fSlow309 - fSlow310;
		for (int i0 = 0; i0 < count; i0 = i0 + 1) {
			float fTemp0 = static_cast<float>(input15[i0]);
			fRec0[0] = fTemp0 / fSlow12 - (fRec0[2] * fSlow17 + 2.0f * fRec0[1] * fSlow16) / fSlow15;
			float fTemp1 = ((iSlow2) ? (fRec0[2] + fRec0[0] + 2.0f * fRec0[1]) / fSlow15 : fTemp0);
			int fVec0_widx_tmp = fVec0_widx;
			fVec0[fVec0_widx_tmp] = fTemp1;
			int fVec0_ridx_tmp0 = fVec0_widx - iSlow19;
			float fTemp2 = static_cast<float>(input14[i0]);
			fRec1[0] = fTemp2 / fSlow29 - (fRec1[2] * fSlow34 + 2.0f * fRec1[1] * fSlow33) / fSlow32;
			float fTemp3 = ((iSlow22) ? (fRec1[2] + fRec1[0] + 2.0f * fRec1[1]) / fSlow32 : fTemp2);
			int fVec1_widx_tmp = fVec1_widx;
			fVec1[fVec1_widx_tmp] = fTemp3;
			int fVec1_ridx_tmp0 = fVec1_widx - iSlow36;
			float fTemp4 = static_cast<float>(input13[i0]);
			fRec2[0] = fTemp4 / fSlow46 - (fRec2[2] * fSlow51 + 2.0f * fRec2[1] * fSlow50) / fSlow49;
			float fTemp5 = ((iSlow39) ? (fRec2[2] + fRec2[0] + 2.0f * fRec2[1]) / fSlow49 : fTemp4);
			int fVec2_widx_tmp = fVec2_widx;
			fVec2[fVec2_widx_tmp] = fTemp5;
			int fVec2_ridx_tmp0 = fVec2_widx - iSlow53;
			float fTemp6 = static_cast<float>(input12[i0]);
			fRec3[0] = fTemp6 / fSlow63 - (fRec3[2] * fSlow68 + 2.0f * fRec3[1] * fSlow67) / fSlow66;
			float fTemp7 = ((iSlow56) ? (fRec3[2] + fRec3[0] + 2.0f * fRec3[1]) / fSlow66 : fTemp6);
			int fVec3_widx_tmp = fVec3_widx;
			fVec3[fVec3_widx_tmp] = fTemp7;
			int fVec3_ridx_tmp0 = fVec3_widx - iSlow70;
			float fTemp8 = static_cast<float>(input11[i0]);
			fRec4[0] = fTemp8 / fSlow80 - (fRec4[2] * fSlow85 + 2.0f * fRec4[1] * fSlow84) / fSlow83;
			float fTemp9 = ((iSlow73) ? (fRec4[2] + fRec4[0] + 2.0f * fRec4[1]) / fSlow83 : fTemp8);
			int fVec4_widx_tmp = fVec4_widx;
			fVec4[fVec4_widx_tmp] = fTemp9;
			int fVec4_ridx_tmp0 = fVec4_widx - iSlow87;
			float fTemp10 = static_cast<float>(input10[i0]);
			fRec5[0] = fTemp10 / fSlow97 - (fRec5[2] * fSlow102 + 2.0f * fRec5[1] * fSlow101) / fSlow100;
			float fTemp11 = ((iSlow90) ? (fRec5[2] + fRec5[0] + 2.0f * fRec5[1]) / fSlow100 : fTemp10);
			int fVec5_widx_tmp = fVec5_widx;
			fVec5[fVec5_widx_tmp] = fTemp11;
			int fVec5_ridx_tmp0 = fVec5_widx - iSlow104;
			float fTemp12 = static_cast<float>(input9[i0]);
			fRec6[0] = fTemp12 / fSlow114 - (fRec6[2] * fSlow119 + 2.0f * fRec6[1] * fSlow118) / fSlow117;
			float fTemp13 = ((iSlow107) ? (fRec6[2] + fRec6[0] + 2.0f * fRec6[1]) / fSlow117 : fTemp12);
			int fVec6_widx_tmp = fVec6_widx;
			fVec6[fVec6_widx_tmp] = fTemp13;
			int fVec6_ridx_tmp0 = fVec6_widx - iSlow121;
			float fTemp14 = static_cast<float>(input8[i0]);
			fRec7[0] = fTemp14 / fSlow131 - (fRec7[2] * fSlow136 + 2.0f * fRec7[1] * fSlow135) / fSlow134;
			float fTemp15 = ((iSlow124) ? (fRec7[2] + fRec7[0] + 2.0f * fRec7[1]) / fSlow134 : fTemp14);
			int fVec7_widx_tmp = fVec7_widx;
			fVec7[fVec7_widx_tmp] = fTemp15;
			int fVec7_ridx_tmp0 = fVec7_widx - iSlow138;
			float fTemp16 = static_cast<float>(input7[i0]);
			fRec8[0] = fTemp16 / fSlow148 - (fRec8[2] * fSlow153 + 2.0f * fRec8[1] * fSlow152) / fSlow151;
			float fTemp17 = ((iSlow141) ? (fRec8[2] + fRec8[0] + 2.0f * fRec8[1]) / fSlow151 : fTemp16);
			int fVec8_widx_tmp = fVec8_widx;
			fVec8[fVec8_widx_tmp] = fTemp17;
			int fVec8_ridx_tmp0 = fVec8_widx - iSlow155;
			float fTemp18 = static_cast<float>(input6[i0]);
			fRec9[0] = fTemp18 / fSlow165 - (fRec9[2] * fSlow170 + 2.0f * fRec9[1] * fSlow169) / fSlow168;
			float fTemp19 = ((iSlow158) ? (fRec9[2] + fRec9[0] + 2.0f * fRec9[1]) / fSlow168 : fTemp18);
			int fVec9_widx_tmp = fVec9_widx;
			fVec9[fVec9_widx_tmp] = fTemp19;
			int fVec9_ridx_tmp0 = fVec9_widx - iSlow172;
			float fTemp20 = static_cast<float>(input5[i0]);
			fRec10[0] = fTemp20 / fSlow182 - (fRec10[2] * fSlow187 + 2.0f * fRec10[1] * fSlow186) / fSlow185;
			float fTemp21 = ((iSlow175) ? (fRec10[2] + fRec10[0] + 2.0f * fRec10[1]) / fSlow185 : fTemp20);
			int fVec10_widx_tmp = fVec10_widx;
			fVec10[fVec10_widx_tmp] = fTemp21;
			int fVec10_ridx_tmp0 = fVec10_widx - iSlow189;
			float fTemp22 = static_cast<float>(input4[i0]);
			fRec11[0] = fTemp22 / fSlow199 - (fRec11[2] * fSlow204 + 2.0f * fRec11[1] * fSlow203) / fSlow202;
			float fTemp23 = ((iSlow192) ? (fRec11[2] + fRec11[0] + 2.0f * fRec11[1]) / fSlow202 : fTemp22);
			int fVec11_widx_tmp = fVec11_widx;
			fVec11[fVec11_widx_tmp] = fTemp23;
			int fVec11_ridx_tmp0 = fVec11_widx - iSlow206;
			float fTemp24 = static_cast<float>(input3[i0]);
			fRec12[0] = fTemp24 / fSlow216 - (fRec12[2] * fSlow221 + 2.0f * fRec12[1] * fSlow220) / fSlow219;
			float fTemp25 = ((iSlow209) ? (fRec12[2] + fRec12[0] + 2.0f * fRec12[1]) / fSlow219 : fTemp24);
			int fVec12_widx_tmp = fVec12_widx;
			fVec12[fVec12_widx_tmp] = fTemp25;
			int fVec12_ridx_tmp0 = fVec12_widx - iSlow223;
			float fTemp26 = static_cast<float>(input2[i0]);
			fRec13[0] = fTemp26 / fSlow233 - (fRec13[2] * fSlow238 + 2.0f * fRec13[1] * fSlow237) / fSlow236;
			float fTemp27 = ((iSlow226) ? (fRec13[2] + fRec13[0] + 2.0f * fRec13[1]) / fSlow236 : fTemp26);
			int fVec13_widx_tmp = fVec13_widx;
			fVec13[fVec13_widx_tmp] = fTemp27;
			int fVec13_ridx_tmp0 = fVec13_widx - iSlow240;
			float fTemp28 = static_cast<float>(input1[i0]);
			fRec14[0] = fTemp28 / fSlow250 - (fRec14[2] * fSlow255 + 2.0f * fRec14[1] * fSlow254) / fSlow253;
			float fTemp29 = ((iSlow243) ? (fRec14[2] + fRec14[0] + 2.0f * fRec14[1]) / fSlow253 : fTemp28);
			int fVec14_widx_tmp = fVec14_widx;
			fVec14[fVec14_widx_tmp] = fTemp29;
			int fVec14_ridx_tmp0 = fVec14_widx - iSlow257;
			float fTemp30 = static_cast<float>(input0[i0]);
			fRec15[0] = fTemp30 / fSlow264 - (fRec15[2] * fSlow269 + 2.0f * fRec15[1] * fSlow268) / fSlow267;
			float fTemp31 = ((iSlow260) ? (fRec15[2] + fRec15[0] + 2.0f * fRec15[1]) / fSlow267 : fTemp30);
			int fVec15_widx_tmp = fVec15_widx;
			fVec15[fVec15_widx_tmp] = fTemp31;
			int fVec15_ridx_tmp0 = fVec15_widx - iSlow272;
			int fVec15_ridx_tmp1 = fVec15_widx - iSlow276;
			int fVec14_ridx_tmp1 = fVec14_widx - iSlow277;
			int fVec13_ridx_tmp1 = fVec13_widx - iSlow279;
			int fVec12_ridx_tmp1 = fVec12_widx - iSlow281;
			int fVec11_ridx_tmp1 = fVec11_widx - iSlow283;
			int fVec10_ridx_tmp1 = fVec10_widx - iSlow285;
			int fVec9_ridx_tmp1 = fVec9_widx - iSlow287;
			int fVec8_ridx_tmp1 = fVec8_widx - iSlow289;
			int fVec7_ridx_tmp1 = fVec7_widx - iSlow291;
			int fVec6_ridx_tmp1 = fVec6_widx - iSlow293;
			int fVec5_ridx_tmp1 = fVec5_widx - iSlow295;
			int fVec4_ridx_tmp1 = fVec4_widx - iSlow297;
			int fVec3_ridx_tmp1 = fVec3_widx - iSlow299;
			int fVec2_ridx_tmp1 = fVec2_widx - iSlow301;
			int fVec1_ridx_tmp1 = fVec1_widx - iSlow303;
			int fVec0_ridx_tmp1 = fVec0_widx - iSlow305;
			output0[i0] = static_cast<FAUSTFLOAT>(fSlow306 * fVec0[((fVec0_ridx_tmp1 < 0) ? fVec0_ridx_tmp1 + 3029 : fVec0_ridx_tmp1)] + fSlow304 * fVec1[((fVec1_ridx_tmp1 < 0) ? fVec1_ridx_tmp1 + 3029 : fVec1_ridx_tmp1)] + fSlow302 * fVec2[((fVec2_ridx_tmp1 < 0) ? fVec2_ridx_tmp1 + 3029 : fVec2_ridx_tmp1)] + fSlow300 * fVec3[((fVec3_ridx_tmp1 < 0) ? fVec3_ridx_tmp1 + 3029 : fVec3_ridx_tmp1)] + fSlow298 * fVec4[((fVec4_ridx_tmp1 < 0) ? fVec4_ridx_tmp1 + 3029 : fVec4_ridx_tmp1)] + fSlow296 * fVec5[((fVec5_ridx_tmp1 < 0) ? fVec5_ridx_tmp1 + 3029 : fVec5_ridx_tmp1)] + fSlow294 * fVec6[((fVec6_ridx_tmp1 < 0) ? fVec6_ridx_tmp1 + 3029 : fVec6_ridx_tmp1)] + fSlow292 * fVec7[((fVec7_ridx_tmp1 < 0) ? fVec7_ridx_tmp1 + 3029 : fVec7_ridx_tmp1)] + fSlow290 * fVec8[((fVec8_ridx_tmp1 < 0) ? fVec8_ridx_tmp1 + 3029 : fVec8_ridx_tmp1)] + fSlow288 * fVec9[((fVec9_ridx_tmp1 < 0) ? fVec9_ridx_tmp1 + 3029 : fVec9_ridx_tmp1)] + fSlow286 * fVec10[((fVec10_ridx_tmp1 < 0) ? fVec10_ridx_tmp1 + 3029 : fVec10_ridx_tmp1)] + fSlow284 * fVec11[((fVec11_ridx_tmp1 < 0) ? fVec11_ridx_tmp1 + 3029 : fVec11_ridx_tmp1)] + fSlow282 * fVec12[((fVec12_ridx_tmp1 < 0) ? fVec12_ridx_tmp1 + 3029 : fVec12_ridx_tmp1)] + fSlow280 * fVec13[((fVec13_ridx_tmp1 < 0) ? fVec13_ridx_tmp1 + 3029 : fVec13_ridx_tmp1)] + fSlow278 * fVec14[((fVec14_ridx_tmp1 < 0) ? fVec14_ridx_tmp1 + 3029 : fVec14_ridx_tmp1)] + fVec15[((fVec15_ridx_tmp1 < 0) ? fVec15_ridx_tmp1 + 3029 : fVec15_ridx_tmp1)] * fSlow275 + fSlow274 * fVec15[((fVec15_ridx_tmp0 < 0) ? fVec15_ridx_tmp0 + 3029 : fVec15_ridx_tmp0)] + fVec14[((fVec14_ridx_tmp0 < 0) ? fVec14_ridx_tmp0 + 3029 : fVec14_ridx_tmp0)] * fSlow249 + fVec13[((fVec13_ridx_tmp0 < 0) ? fVec13_ridx_tmp0 + 3029 : fVec13_ridx_tmp0)] * fSlow232 + fVec12[((fVec12_ridx_tmp0 < 0) ? fVec12_ridx_tmp0 + 3029 : fVec12_ridx_tmp0)] * fSlow215 + fVec11[((fVec11_ridx_tmp0 < 0) ? fVec11_ridx_tmp0 + 3029 : fVec11_ridx_tmp0)] * fSlow198 + fVec10[((fVec10_ridx_tmp0 < 0) ? fVec10_ridx_tmp0 + 3029 : fVec10_ridx_tmp0)] * fSlow181 + fVec9[((fVec9_ridx_tmp0 < 0) ? fVec9_ridx_tmp0 + 3029 : fVec9_ridx_tmp0)] * fSlow164 + fVec8[((fVec8_ridx_tmp0 < 0) ? fVec8_ridx_tmp0 + 3029 : fVec8_ridx_tmp0)] * fSlow147 + fVec7[((fVec7_ridx_tmp0 < 0) ? fVec7_ridx_tmp0 + 3029 : fVec7_ridx_tmp0)] * fSlow130 + fVec6[((fVec6_ridx_tmp0 < 0) ? fVec6_ridx_tmp0 + 3029 : fVec6_ridx_tmp0)] * fSlow113 + fVec5[((fVec5_ridx_tmp0 < 0) ? fVec5_ridx_tmp0 + 3029 : fVec5_ridx_tmp0)] * fSlow96 + fVec4[((fVec4_ridx_tmp0 < 0) ? fVec4_ridx_tmp0 + 3029 : fVec4_ridx_tmp0)] * fSlow79 + fVec3[((fVec3_ridx_tmp0 < 0) ? fVec3_ridx_tmp0 + 3029 : fVec3_ridx_tmp0)] * fSlow62 + fVec2[((fVec2_ridx_tmp0 < 0) ? fVec2_ridx_tmp0 + 3029 : fVec2_ridx_tmp0)] * fSlow45 + fVec1[((fVec1_ridx_tmp0 < 0) ? fVec1_ridx_tmp0 + 3029 : fVec1_ridx_tmp0)] * fSlow28 + fVec0[((fVec0_ridx_tmp0 < 0) ? fVec0_ridx_tmp0 + 3029 : fVec0_ridx_tmp0)] * fSlow11);
			fRec16[0] = fTemp0 / fSlow312 - (fRec16[2] * fSlow317 + 2.0f * fRec16[1] * fSlow316) / fSlow315;
			float fTemp32 = ((iSlow2) ? (fRec16[2] + fRec16[0] + 2.0f * fRec16[1]) / fSlow315 : fTemp0);
			int fVec16_widx_tmp = fVec16_widx;
			fVec16[fVec16_widx_tmp] = fTemp32;
			int fVec16_ridx_tmp0 = fVec16_widx - iSlow319;
			fRec17[0] = fTemp2 / fSlow324 - (fRec17[2] * fSlow329 + 2.0f * fRec17[1] * fSlow328) / fSlow327;
			float fTemp33 = ((iSlow22) ? (fRec17[2] + fRec17[0] + 2.0f * fRec17[1]) / fSlow327 : fTemp2);
			int fVec17_widx_tmp = fVec17_widx;
			fVec17[fVec17_widx_tmp] = fTemp33;
			int fVec17_ridx_tmp0 = fVec17_widx - iSlow331;
			fRec18[0] = fTemp4 / fSlow336 - (fRec18[2] * fSlow341 + 2.0f * fRec18[1] * fSlow340) / fSlow339;
			float fTemp34 = ((iSlow39) ? (fRec18[2] + fRec18[0] + 2.0f * fRec18[1]) / fSlow339 : fTemp4);
			int fVec18_widx_tmp = fVec18_widx;
			fVec18[fVec18_widx_tmp] = fTemp34;
			int fVec18_ridx_tmp0 = fVec18_widx - iSlow343;
			fRec19[0] = fTemp6 / fSlow348 - (fRec19[2] * fSlow353 + 2.0f * fRec19[1] * fSlow352) / fSlow351;
			float fTemp35 = ((iSlow56) ? (fRec19[2] + fRec19[0] + 2.0f * fRec19[1]) / fSlow351 : fTemp6);
			int fVec19_widx_tmp = fVec19_widx;
			fVec19[fVec19_widx_tmp] = fTemp35;
			int fVec19_ridx_tmp0 = fVec19_widx - iSlow355;
			fRec20[0] = fTemp8 / fSlow360 - (fRec20[2] * fSlow365 + 2.0f * fRec20[1] * fSlow364) / fSlow363;
			float fTemp36 = ((iSlow73) ? (fRec20[2] + fRec20[0] + 2.0f * fRec20[1]) / fSlow363 : fTemp8);
			int fVec20_widx_tmp = fVec20_widx;
			fVec20[fVec20_widx_tmp] = fTemp36;
			int fVec20_ridx_tmp0 = fVec20_widx - iSlow367;
			fRec21[0] = fTemp10 / fSlow372 - (fRec21[2] * fSlow377 + 2.0f * fRec21[1] * fSlow376) / fSlow375;
			float fTemp37 = ((iSlow90) ? (fRec21[2] + fRec21[0] + 2.0f * fRec21[1]) / fSlow375 : fTemp10);
			int fVec21_widx_tmp = fVec21_widx;
			fVec21[fVec21_widx_tmp] = fTemp37;
			int fVec21_ridx_tmp0 = fVec21_widx - iSlow379;
			fRec22[0] = fTemp12 / fSlow384 - (fRec22[2] * fSlow389 + 2.0f * fRec22[1] * fSlow388) / fSlow387;
			float fTemp38 = ((iSlow107) ? (fRec22[2] + fRec22[0] + 2.0f * fRec22[1]) / fSlow387 : fTemp12);
			int fVec22_widx_tmp = fVec22_widx;
			fVec22[fVec22_widx_tmp] = fTemp38;
			int fVec22_ridx_tmp0 = fVec22_widx - iSlow391;
			fRec23[0] = fTemp14 / fSlow396 - (fRec23[2] * fSlow401 + 2.0f * fRec23[1] * fSlow400) / fSlow399;
			float fTemp39 = ((iSlow124) ? (fRec23[2] + fRec23[0] + 2.0f * fRec23[1]) / fSlow399 : fTemp14);
			int fVec23_widx_tmp = fVec23_widx;
			fVec23[fVec23_widx_tmp] = fTemp39;
			int fVec23_ridx_tmp0 = fVec23_widx - iSlow403;
			fRec24[0] = fTemp16 / fSlow408 - (fRec24[2] * fSlow413 + 2.0f * fRec24[1] * fSlow412) / fSlow411;
			float fTemp40 = ((iSlow141) ? (fRec24[2] + fRec24[0] + 2.0f * fRec24[1]) / fSlow411 : fTemp16);
			int fVec24_widx_tmp = fVec24_widx;
			fVec24[fVec24_widx_tmp] = fTemp40;
			int fVec24_ridx_tmp0 = fVec24_widx - iSlow415;
			fRec25[0] = fTemp18 / fSlow420 - (fRec25[2] * fSlow425 + 2.0f * fRec25[1] * fSlow424) / fSlow423;
			float fTemp41 = ((iSlow158) ? (fRec25[2] + fRec25[0] + 2.0f * fRec25[1]) / fSlow423 : fTemp18);
			int fVec25_widx_tmp = fVec25_widx;
			fVec25[fVec25_widx_tmp] = fTemp41;
			int fVec25_ridx_tmp0 = fVec25_widx - iSlow427;
			fRec26[0] = fTemp20 / fSlow432 - (fRec26[2] * fSlow437 + 2.0f * fRec26[1] * fSlow436) / fSlow435;
			float fTemp42 = ((iSlow175) ? (fRec26[2] + fRec26[0] + 2.0f * fRec26[1]) / fSlow435 : fTemp20);
			int fVec26_widx_tmp = fVec26_widx;
			fVec26[fVec26_widx_tmp] = fTemp42;
			int fVec26_ridx_tmp0 = fVec26_widx - iSlow439;
			fRec27[0] = fTemp22 / fSlow444 - (fRec27[2] * fSlow449 + 2.0f * fRec27[1] * fSlow448) / fSlow447;
			float fTemp43 = ((iSlow192) ? (fRec27[2] + fRec27[0] + 2.0f * fRec27[1]) / fSlow447 : fTemp22);
			int fVec27_widx_tmp = fVec27_widx;
			fVec27[fVec27_widx_tmp] = fTemp43;
			int fVec27_ridx_tmp0 = fVec27_widx - iSlow451;
			fRec28[0] = fTemp24 / fSlow456 - (fRec28[2] * fSlow461 + 2.0f * fRec28[1] * fSlow460) / fSlow459;
			float fTemp44 = ((iSlow209) ? (fRec28[2] + fRec28[0] + 2.0f * fRec28[1]) / fSlow459 : fTemp24);
			int fVec28_widx_tmp = fVec28_widx;
			fVec28[fVec28_widx_tmp] = fTemp44;
			int fVec28_ridx_tmp0 = fVec28_widx - iSlow463;
			fRec29[0] = fTemp26 / fSlow468 - (fRec29[2] * fSlow473 + 2.0f * fRec29[1] * fSlow472) / fSlow471;
			float fTemp45 = ((iSlow226) ? (fRec29[2] + fRec29[0] + 2.0f * fRec29[1]) / fSlow471 : fTemp26);
			int fVec29_widx_tmp = fVec29_widx;
			fVec29[fVec29_widx_tmp] = fTemp45;
			int fVec29_ridx_tmp0 = fVec29_widx - iSlow475;
			fRec30[0] = fTemp28 / fSlow480 - (fRec30[2] * fSlow485 + 2.0f * fRec30[1] * fSlow484) / fSlow483;
			float fTemp46 = ((iSlow243) ? (fRec30[2] + fRec30[0] + 2.0f * fRec30[1]) / fSlow483 : fTemp28);
			int fVec30_widx_tmp = fVec30_widx;
			fVec30[fVec30_widx_tmp] = fTemp46;
			int fVec30_ridx_tmp0 = fVec30_widx - iSlow487;
			fRec31[0] = fTemp30 / fSlow489 - (fRec31[2] * fSlow494 + 2.0f * fRec31[1] * fSlow493) / fSlow492;
			float fTemp47 = ((iSlow260) ? (fRec31[2] + fRec31[0] + 2.0f * fRec31[1]) / fSlow492 : fTemp30);
			int fVec31_widx_tmp = fVec31_widx;
			fVec31[fVec31_widx_tmp] = fTemp47;
			int fVec31_ridx_tmp0 = fVec31_widx - iSlow497;
			int fVec31_ridx_tmp1 = fVec31_widx - iSlow501;
			int fVec30_ridx_tmp1 = fVec30_widx - iSlow502;
			int fVec29_ridx_tmp1 = fVec29_widx - iSlow504;
			int fVec28_ridx_tmp1 = fVec28_widx - iSlow506;
			int fVec27_ridx_tmp1 = fVec27_widx - iSlow508;
			int fVec26_ridx_tmp1 = fVec26_widx - iSlow510;
			int fVec25_ridx_tmp1 = fVec25_widx - iSlow512;
			int fVec24_ridx_tmp1 = fVec24_widx - iSlow514;
			int fVec23_ridx_tmp1 = fVec23_widx - iSlow516;
			int fVec22_ridx_tmp1 = fVec22_widx - iSlow518;
			int fVec21_ridx_tmp1 = fVec21_widx - iSlow520;
			int fVec20_ridx_tmp1 = fVec20_widx - iSlow522;
			int fVec19_ridx_tmp1 = fVec19_widx - iSlow524;
			int fVec18_ridx_tmp1 = fVec18_widx - iSlow526;
			int fVec17_ridx_tmp1 = fVec17_widx - iSlow528;
			int fVec16_ridx_tmp1 = fVec16_widx - iSlow530;
			output1[i0] = static_cast<FAUSTFLOAT>(fSlow531 * fVec16[((fVec16_ridx_tmp1 < 0) ? fVec16_ridx_tmp1 + 3029 : fVec16_ridx_tmp1)] + fSlow529 * fVec17[((fVec17_ridx_tmp1 < 0) ? fVec17_ridx_tmp1 + 3029 : fVec17_ridx_tmp1)] + fSlow527 * fVec18[((fVec18_ridx_tmp1 < 0) ? fVec18_ridx_tmp1 + 3029 : fVec18_ridx_tmp1)] + fSlow525 * fVec19[((fVec19_ridx_tmp1 < 0) ? fVec19_ridx_tmp1 + 3029 : fVec19_ridx_tmp1)] + fSlow523 * fVec20[((fVec20_ridx_tmp1 < 0) ? fVec20_ridx_tmp1 + 3029 : fVec20_ridx_tmp1)] + fSlow521 * fVec21[((fVec21_ridx_tmp1 < 0) ? fVec21_ridx_tmp1 + 3029 : fVec21_ridx_tmp1)] + fSlow519 * fVec22[((fVec22_ridx_tmp1 < 0) ? fVec22_ridx_tmp1 + 3029 : fVec22_ridx_tmp1)] + fSlow517 * fVec23[((fVec23_ridx_tmp1 < 0) ? fVec23_ridx_tmp1 + 3029 : fVec23_ridx_tmp1)] + fSlow515 * fVec24[((fVec24_ridx_tmp1 < 0) ? fVec24_ridx_tmp1 + 3029 : fVec24_ridx_tmp1)] + fSlow513 * fVec25[((fVec25_ridx_tmp1 < 0) ? fVec25_ridx_tmp1 + 3029 : fVec25_ridx_tmp1)] + fSlow511 * fVec26[((fVec26_ridx_tmp1 < 0) ? fVec26_ridx_tmp1 + 3029 : fVec26_ridx_tmp1)] + fSlow509 * fVec27[((fVec27_ridx_tmp1 < 0) ? fVec27_ridx_tmp1 + 3029 : fVec27_ridx_tmp1)] + fSlow507 * fVec28[((fVec28_ridx_tmp1 < 0) ? fVec28_ridx_tmp1 + 3029 : fVec28_ridx_tmp1)] + fSlow505 * fVec29[((fVec29_ridx_tmp1 < 0) ? fVec29_ridx_tmp1 + 3029 : fVec29_ridx_tmp1)] + fSlow503 * fVec30[((fVec30_ridx_tmp1 < 0) ? fVec30_ridx_tmp1 + 3029 : fVec30_ridx_tmp1)] + fVec31[((fVec31_ridx_tmp1 < 0) ? fVec31_ridx_tmp1 + 3029 : fVec31_ridx_tmp1)] * fSlow500 + fSlow499 * fVec31[((fVec31_ridx_tmp0 < 0) ? fVec31_ridx_tmp0 + 3029 : fVec31_ridx_tmp0)] + fVec30[((fVec30_ridx_tmp0 < 0) ? fVec30_ridx_tmp0 + 3029 : fVec30_ridx_tmp0)] * fSlow479 + fVec29[((fVec29_ridx_tmp0 < 0) ? fVec29_ridx_tmp0 + 3029 : fVec29_ridx_tmp0)] * fSlow467 + fVec28[((fVec28_ridx_tmp0 < 0) ? fVec28_ridx_tmp0 + 3029 : fVec28_ridx_tmp0)] * fSlow455 + fVec27[((fVec27_ridx_tmp0 < 0) ? fVec27_ridx_tmp0 + 3029 : fVec27_ridx_tmp0)] * fSlow443 + fVec26[((fVec26_ridx_tmp0 < 0) ? fVec26_ridx_tmp0 + 3029 : fVec26_ridx_tmp0)] * fSlow431 + fVec25[((fVec25_ridx_tmp0 < 0) ? fVec25_ridx_tmp0 + 3029 : fVec25_ridx_tmp0)] * fSlow419 + fVec24[((fVec24_ridx_tmp0 < 0) ? fVec24_ridx_tmp0 + 3029 : fVec24_ridx_tmp0)] * fSlow407 + fVec23[((fVec23_ridx_tmp0 < 0) ? fVec23_ridx_tmp0 + 3029 : fVec23_ridx_tmp0)] * fSlow395 + fVec22[((fVec22_ridx_tmp0 < 0) ? fVec22_ridx_tmp0 + 3029 : fVec22_ridx_tmp0)] * fSlow383 + fVec21[((fVec21_ridx_tmp0 < 0) ? fVec21_ridx_tmp0 + 3029 : fVec21_ridx_tmp0)] * fSlow371 + fVec20[((fVec20_ridx_tmp0 < 0) ? fVec20_ridx_tmp0 + 3029 : fVec20_ridx_tmp0)] * fSlow359 + fVec19[((fVec19_ridx_tmp0 < 0) ? fVec19_ridx_tmp0 + 3029 : fVec19_ridx_tmp0)] * fSlow347 + fVec18[((fVec18_ridx_tmp0 < 0) ? fVec18_ridx_tmp0 + 3029 : fVec18_ridx_tmp0)] * fSlow335 + fVec17[((fVec17_ridx_tmp0 < 0) ? fVec17_ridx_tmp0 + 3029 : fVec17_ridx_tmp0)] * fSlow323 + fVec16[((fVec16_ridx_tmp0 < 0) ? fVec16_ridx_tmp0 + 3029 : fVec16_ridx_tmp0)] * fSlow311);
			fRec0[2] = fRec0[1];
			fRec0[1] = fRec0[0];
			fVec0_widx_tmp = fVec0_widx_tmp + 1;
			fVec0_widx_tmp = ((fVec0_widx_tmp == 3029) ? 0 : fVec0_widx_tmp);
			fVec0_widx = fVec0_widx_tmp;
			fRec1[2] = fRec1[1];
			fRec1[1] = fRec1[0];
			fVec1_widx_tmp = fVec1_widx_tmp + 1;
			fVec1_widx_tmp = ((fVec1_widx_tmp == 3029) ? 0 : fVec1_widx_tmp);
			fVec1_widx = fVec1_widx_tmp;
			fRec2[2] = fRec2[1];
			fRec2[1] = fRec2[0];
			fVec2_widx_tmp = fVec2_widx_tmp + 1;
			fVec2_widx_tmp = ((fVec2_widx_tmp == 3029) ? 0 : fVec2_widx_tmp);
			fVec2_widx = fVec2_widx_tmp;
			fRec3[2] = fRec3[1];
			fRec3[1] = fRec3[0];
			fVec3_widx_tmp = fVec3_widx_tmp + 1;
			fVec3_widx_tmp = ((fVec3_widx_tmp == 3029) ? 0 : fVec3_widx_tmp);
			fVec3_widx = fVec3_widx_tmp;
			fRec4[2] = fRec4[1];
			fRec4[1] = fRec4[0];
			fVec4_widx_tmp = fVec4_widx_tmp + 1;
			fVec4_widx_tmp = ((fVec4_widx_tmp == 3029) ? 0 : fVec4_widx_tmp);
			fVec4_widx = fVec4_widx_tmp;
			fRec5[2] = fRec5[1];
			fRec5[1] = fRec5[0];
			fVec5_widx_tmp = fVec5_widx_tmp + 1;
			fVec5_widx_tmp = ((fVec5_widx_tmp == 3029) ? 0 : fVec5_widx_tmp);
			fVec5_widx = fVec5_widx_tmp;
			fRec6[2] = fRec6[1];
			fRec6[1] = fRec6[0];
			fVec6_widx_tmp = fVec6_widx_tmp + 1;
			fVec6_widx_tmp = ((fVec6_widx_tmp == 3029) ? 0 : fVec6_widx_tmp);
			fVec6_widx = fVec6_widx_tmp;
			fRec7[2] = fRec7[1];
			fRec7[1] = fRec7[0];
			fVec7_widx_tmp = fVec7_widx_tmp + 1;
			fVec7_widx_tmp = ((fVec7_widx_tmp == 3029) ? 0 : fVec7_widx_tmp);
			fVec7_widx = fVec7_widx_tmp;
			fRec8[2] = fRec8[1];
			fRec8[1] = fRec8[0];
			fVec8_widx_tmp = fVec8_widx_tmp + 1;
			fVec8_widx_tmp = ((fVec8_widx_tmp == 3029) ? 0 : fVec8_widx_tmp);
			fVec8_widx = fVec8_widx_tmp;
			fRec9[2] = fRec9[1];
			fRec9[1] = fRec9[0];
			fVec9_widx_tmp = fVec9_widx_tmp + 1;
			fVec9_widx_tmp = ((fVec9_widx_tmp == 3029) ? 0 : fVec9_widx_tmp);
			fVec9_widx = fVec9_widx_tmp;
			fRec10[2] = fRec10[1];
			fRec10[1] = fRec10[0];
			fVec10_widx_tmp = fVec10_widx_tmp + 1;
			fVec10_widx_tmp = ((fVec10_widx_tmp == 3029) ? 0 : fVec10_widx_tmp);
			fVec10_widx = fVec10_widx_tmp;
			fRec11[2] = fRec11[1];
			fRec11[1] = fRec11[0];
			fVec11_widx_tmp = fVec11_widx_tmp + 1;
			fVec11_widx_tmp = ((fVec11_widx_tmp == 3029) ? 0 : fVec11_widx_tmp);
			fVec11_widx = fVec11_widx_tmp;
			fRec12[2] = fRec12[1];
			fRec12[1] = fRec12[0];
			fVec12_widx_tmp = fVec12_widx_tmp + 1;
			fVec12_widx_tmp = ((fVec12_widx_tmp == 3029) ? 0 : fVec12_widx_tmp);
			fVec12_widx = fVec12_widx_tmp;
			fRec13[2] = fRec13[1];
			fRec13[1] = fRec13[0];
			fVec13_widx_tmp = fVec13_widx_tmp + 1;
			fVec13_widx_tmp = ((fVec13_widx_tmp == 3029) ? 0 : fVec13_widx_tmp);
			fVec13_widx = fVec13_widx_tmp;
			fRec14[2] = fRec14[1];
			fRec14[1] = fRec14[0];
			fVec14_widx_tmp = fVec14_widx_tmp + 1;
			fVec14_widx_tmp = ((fVec14_widx_tmp == 3029) ? 0 : fVec14_widx_tmp);
			fVec14_widx = fVec14_widx_tmp;
			fRec15[2] = fRec15[1];
			fRec15[1] = fRec15[0];
			fVec15_widx_tmp = fVec15_widx_tmp + 1;
			fVec15_widx_tmp = ((fVec15_widx_tmp == 3029) ? 0 : fVec15_widx_tmp);
			fVec15_widx = fVec15_widx_tmp;
			fRec16[2] = fRec16[1];
			fRec16[1] = fRec16[0];
			fVec16_widx_tmp = fVec16_widx_tmp + 1;
			fVec16_widx_tmp = ((fVec16_widx_tmp == 3029) ? 0 : fVec16_widx_tmp);
			fVec16_widx = fVec16_widx_tmp;
			fRec17[2] = fRec17[1];
			fRec17[1] = fRec17[0];
			fVec17_widx_tmp = fVec17_widx_tmp + 1;
			fVec17_widx_tmp = ((fVec17_widx_tmp == 3029) ? 0 : fVec17_widx_tmp);
			fVec17_widx = fVec17_widx_tmp;
			fRec18[2] = fRec18[1];
			fRec18[1] = fRec18[0];
			fVec18_widx_tmp = fVec18_widx_tmp + 1;
			fVec18_widx_tmp = ((fVec18_widx_tmp == 3029) ? 0 : fVec18_widx_tmp);
			fVec18_widx = fVec18_widx_tmp;
			fRec19[2] = fRec19[1];
			fRec19[1] = fRec19[0];
			fVec19_widx_tmp = fVec19_widx_tmp + 1;
			fVec19_widx_tmp = ((fVec19_widx_tmp == 3029) ? 0 : fVec19_widx_tmp);
			fVec19_widx = fVec19_widx_tmp;
			fRec20[2] = fRec20[1];
			fRec20[1] = fRec20[0];
			fVec20_widx_tmp = fVec20_widx_tmp + 1;
			fVec20_widx_tmp = ((fVec20_widx_tmp == 3029) ? 0 : fVec20_widx_tmp);
			fVec20_widx = fVec20_widx_tmp;
			fRec21[2] = fRec21[1];
			fRec21[1] = fRec21[0];
			fVec21_widx_tmp = fVec21_widx_tmp + 1;
			fVec21_widx_tmp = ((fVec21_widx_tmp == 3029) ? 0 : fVec21_widx_tmp);
			fVec21_widx = fVec21_widx_tmp;
			fRec22[2] = fRec22[1];
			fRec22[1] = fRec22[0];
			fVec22_widx_tmp = fVec22_widx_tmp + 1;
			fVec22_widx_tmp = ((fVec22_widx_tmp == 3029) ? 0 : fVec22_widx_tmp);
			fVec22_widx = fVec22_widx_tmp;
			fRec23[2] = fRec23[1];
			fRec23[1] = fRec23[0];
			fVec23_widx_tmp = fVec23_widx_tmp + 1;
			fVec23_widx_tmp = ((fVec23_widx_tmp == 3029) ? 0 : fVec23_widx_tmp);
			fVec23_widx = fVec23_widx_tmp;
			fRec24[2] = fRec24[1];
			fRec24[1] = fRec24[0];
			fVec24_widx_tmp = fVec24_widx_tmp + 1;
			fVec24_widx_tmp = ((fVec24_widx_tmp == 3029) ? 0 : fVec24_widx_tmp);
			fVec24_widx = fVec24_widx_tmp;
			fRec25[2] = fRec25[1];
			fRec25[1] = fRec25[0];
			fVec25_widx_tmp = fVec25_widx_tmp + 1;
			fVec25_widx_tmp = ((fVec25_widx_tmp == 3029) ? 0 : fVec25_widx_tmp);
			fVec25_widx = fVec25_widx_tmp;
			fRec26[2] = fRec26[1];
			fRec26[1] = fRec26[0];
			fVec26_widx_tmp = fVec26_widx_tmp + 1;
			fVec26_widx_tmp = ((fVec26_widx_tmp == 3029) ? 0 : fVec26_widx_tmp);
			fVec26_widx = fVec26_widx_tmp;
			fRec27[2] = fRec27[1];
			fRec27[1] = fRec27[0];
			fVec27_widx_tmp = fVec27_widx_tmp + 1;
			fVec27_widx_tmp = ((fVec27_widx_tmp == 3029) ? 0 : fVec27_widx_tmp);
			fVec27_widx = fVec27_widx_tmp;
			fRec28[2] = fRec28[1];
			fRec28[1] = fRec28[0];
			fVec28_widx_tmp = fVec28_widx_tmp + 1;
			fVec28_widx_tmp = ((fVec28_widx_tmp == 3029) ? 0 : fVec28_widx_tmp);
			fVec28_widx = fVec28_widx_tmp;
			fRec29[2] = fRec29[1];
			fRec29[1] = fRec29[0];
			fVec29_widx_tmp = fVec29_widx_tmp + 1;
			fVec29_widx_tmp = ((fVec29_widx_tmp == 3029) ? 0 : fVec29_widx_tmp);
			fVec29_widx = fVec29_widx_tmp;
			fRec30[2] = fRec30[1];
			fRec30[1] = fRec30[0];
			fVec30_widx_tmp = fVec30_widx_tmp + 1;
			fVec30_widx_tmp = ((fVec30_widx_tmp == 3029) ? 0 : fVec30_widx_tmp);
			fVec30_widx = fVec30_widx_tmp;
			fRec31[2] = fRec31[1];
			fRec31[1] = fRec31[0];
			fVec31_widx_tmp = fVec31_widx_tmp + 1;
			fVec31_widx_tmp = ((fVec31_widx_tmp == 3029) ? 0 : fVec31_widx_tmp);
			fVec31_widx = fVec31_widx_tmp;
		}
	}

};

#ifdef FAUST_UIMACROS
	
	#define FAUST_FILE_NAME "wfs.dsp"
	#define FAUST_CLASS_NAME "mydsp"
	#define FAUST_COMPILATION_OPIONS "-a ananas.cpp -lang cpp -i -fpga-mem-th 4 -ct 1 -dtl 1024 -es 1 -mcd 16 -mdd 1024 -mdy 33 -uim -single -ftz 0"
	#define FAUST_INPUTS 16
	#define FAUST_OUTPUTS 2
	#define FAUST_ACTIVES 34
	#define FAUST_PASSIVES 0

	FAUST_ADDHORIZONTALSLIDER("0/x", fHslider33, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("0/y", fHslider32, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("1/x", fHslider31, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("1/y", fHslider30, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("10/x", fHslider13, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("10/y", fHslider12, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("11/x", fHslider11, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("11/y", fHslider10, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("12/x", fHslider9, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("12/y", fHslider8, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("13/x", fHslider7, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("13/y", fHslider6, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("14/x", fHslider5, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("14/y", fHslider4, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("15/x", fHslider2, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("15/y", fHslider0, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("2/x", fHslider29, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("2/y", fHslider28, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("3/x", fHslider27, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("3/y", fHslider26, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("4/x", fHslider25, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("4/y", fHslider24, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("5/x", fHslider23, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("5/y", fHslider22, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("6/x", fHslider21, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("6/y", fHslider20, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("7/x", fHslider19, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("7/y", fHslider18, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("8/x", fHslider17, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("8/y", fHslider16, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("9/x", fHslider15, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("9/y", fHslider14, 0.0f, -1.0f, 1.0f, 0.001f);
	FAUST_ADDHORIZONTALSLIDER("Global settings/moduleID", fHslider1, -1.0f, -1.0f, 7.0f, 1.0f);
	FAUST_ADDHORIZONTALSLIDER("Global settings/spacing", fHslider3, 0.2f, 0.05f, 0.3f, 0.01f);

	#define FAUST_LIST_ACTIVES(p) \
		p(HORIZONTALSLIDER, 0/x, "0/x", fHslider33, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 0/y, "0/y", fHslider32, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 1/x, "1/x", fHslider31, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 1/y, "1/y", fHslider30, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 10/x, "10/x", fHslider13, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 10/y, "10/y", fHslider12, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 11/x, "11/x", fHslider11, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 11/y, "11/y", fHslider10, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 12/x, "12/x", fHslider9, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 12/y, "12/y", fHslider8, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 13/x, "13/x", fHslider7, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 13/y, "13/y", fHslider6, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 14/x, "14/x", fHslider5, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 14/y, "14/y", fHslider4, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 15/x, "15/x", fHslider2, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 15/y, "15/y", fHslider0, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 2/x, "2/x", fHslider29, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 2/y, "2/y", fHslider28, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 3/x, "3/x", fHslider27, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 3/y, "3/y", fHslider26, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 4/x, "4/x", fHslider25, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 4/y, "4/y", fHslider24, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 5/x, "5/x", fHslider23, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 5/y, "5/y", fHslider22, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 6/x, "6/x", fHslider21, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 6/y, "6/y", fHslider20, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 7/x, "7/x", fHslider19, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 7/y, "7/y", fHslider18, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 8/x, "8/x", fHslider17, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 8/y, "8/y", fHslider16, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 9/x, "9/x", fHslider15, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, 9/y, "9/y", fHslider14, 0.0f, -1.0f, 1.0f, 0.001f) \
		p(HORIZONTALSLIDER, moduleID, "Global settings/moduleID", fHslider1, -1.0f, -1.0f, 7.0f, 1.0f) \
		p(HORIZONTALSLIDER, spacing, "Global settings/spacing", fHslider3, 0.2f, 0.05f, 0.3f, 0.01f) \

	#define FAUST_LIST_PASSIVES(p) \

#endif

/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/

#define MULT_16 32767
#define DIV_16 0.0000305185

#if MIDICTRL
std::list<GUI*> GUI::fGuiList;
ztimedmap GUI::gTimedZoneMap;
#endif

wfs::wfs()
{
#ifdef NVOICES
    int nvoices = NVOICES;
    fDSP = new mydsp_poly(new mydsp(), nvoices, true, true);
#else
    fDSP = new mydsp();
#endif

    // allocating Faust inputs
    if (fDSP->getNumInputs() > 0) {
        fInChannel = new float *[fDSP->getNumInputs()];
    } else {
        fInChannel = nullptr;
    }

    // allocating Faust outputs
    if (fDSP->getNumOutputs() > 0) {
        fOutChannel = new float *[fDSP->getNumOutputs()];
    } else {
        fOutChannel = nullptr;
    }

    fUI = new MapUI();

#if MIDICTRL
    fMIDIHandler = new ananas_midi();
    fMIDIInterface = new MidiUI(fMIDIHandler);
#endif
}

wfs::~wfs()
{
    delete fDSP;
    delete fUI;
    for (int i = 0; i < fDSP->getNumInputs(); i++) {
        delete[] fInChannel[i];
    }
    delete [] fInChannel;
    for (int i = 0; i < fDSP->getNumOutputs(); i++) {
        delete[] fOutChannel[i];
    }
    delete [] fOutChannel;
#if MIDICTRL
    delete fMIDIInterface;
    delete fMIDIHandler;
#endif
}

void wfs::prepare(const uint sampleRate)
{
    fDSP->init(static_cast<int>(sampleRate));
    fDSP->buildUserInterface(fUI);

////    Serial.println("Setting up channel pointers.");
//    // Set up channel pointers
//    for (size_t i{0}; i < ananas::Constants::MaxChannels; ++i) {
//        fInChannel[i] = fInChannelData[i];
//    }
//    for (size_t i{0}; i < ananas::Constants::NumOutputChannels; ++i) {
//        fOutChannel[i] = fOutChannelData[i];
//    }
////    Serial.println("Done setting up channel pointers.");

    // allocating Faust inputs
    if (fDSP->getNumInputs() > 0) {
        for (int i = 0; i < fDSP->getNumInputs(); i++) {
            fInChannel[i] = new float[ananas::Constants::AudioBlockFrames];
        }
    }

    // allocating Faust outputs
    if (fDSP->getNumOutputs() > 0) {
        for (int i = 0; i < fDSP->getNumOutputs(); i++) {
            fOutChannel[i] = new float[ananas::Constants::AudioBlockFrames];
        }
    }

#if MIDICTRL
    fDSP->buildUserInterface(fMIDIInterface);
    fMIDIInterface->run();
#endif
}

void wfs::run() {}

void wfs::beginImpl() {}

void wfs::processImpl(int16_t **inputBuffer, int16_t **outputBuffer, size_t numFrames)
{
    if (getNumInputs() > 0) {
        for (size_t channel{0}; channel < getNumInputs(); channel++) {
            for (size_t i{0}; i < numFrames; i++) {
                int16_t val = inputBuffer[channel][i];
                fInChannel[channel][i] = val * DIV_16;
            }
        }
    }

    fDSP->compute(numFrames, fInChannel, fOutChannel);

    for (size_t channel{0}; channel < getNumOutputs(); channel++) {
        for (size_t i{0}; i < numFrames; i++) {
            int16_t val = fOutChannel[channel][i] * MULT_16;
            outputBuffer[channel][i] = val;
        }
    }
}

void wfs::setParamValue(const std::string &path, float value) const
{
    fUI->setParamValue(path, value);
}

float wfs::getParamValue(const std::string &path) const
{
    return fUI->getParamValue(path);
}

size_t wfs::printTo(Print &p) const
{
    return p.print("wfs:               ") +
         AudioProcessor::printTo(p) +
         p.printf("  Module ID: %d\n",
                  static_cast<int>(fUI->getParamValue("moduleID")));
}

size_t wfs::getNumInputs() const
{
    return ananas::Constants::MaxChannels;
}

size_t wfs::getNumOutputs() const
{
    return ananas::Constants::NumOutputChannels;
}

/********************END ARCHITECTURE SECTION (part 2/2)****************/

#endif
