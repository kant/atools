/*****************************************************************************
* Copyright 2015-2016 Alexander Barthel albar965@mailbox.org
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef FSPATHS_H
#define FSPATHS_H

#include <QString>

namespace atools {
namespace fs {

enum SimulatorType
{
  /* Force numeric values since these are used as indexes */
  /* Platform: FSX, FSX XPack, FSX Gold */
  FSX = 0,

  /* Platform: FSX Steam Edition */
  FSX_SE = 1,

  /* Platform: Prepar3d Version 2 */
  P3D_V2 = 2,

  /* Platform: Prepar3d Version 3 */
  P3D_V3 = 3
};

class FsPaths
{
public:
  /* Get installation path to fsx.exe, etc. */
  static QString getBasePath(SimulatorType type);

  /* Get full path to language dependent "Flight Simulator X Files" or "Flight Simulator X-Dateien", etc. */
  static QString getFilesPath(SimulatorType type);

  /* Path to scenery.cfg */
  static QString getSceneryLibraryPath(SimulatorType type);

private:
  FsPaths()
  {

  }

  /* registry path and key if running on Windows */
  /* Platform: FSX, FSX XPack, FSX Gold */
  static const char *FSX_REGISTRY_PATH;
  static const char *FSX_REGISTRY_KEY;

  /* Platform: FSX Steam Edition */
  static const char *FSX_SE_REGISTRY_PATH;
  static const char *FSX_SE_REGISTRY_KEY;

  /* Platform: Prepar3d Version 2 */
  static const char *P3D_V2_REGISTRY_PATH;
  static const char *P3D_V2_REGISTRY_KEY;

  /* Platform: Prepar3d Version 3 */
  static const char *P3D_V3_REGISTRY_PATH;
  static const char *P3D_V3_REGISTRY_KEY;

  /* Use this as fallback from the settings if not running on Windows */
  static const char *SETTINGS_FSX_PATH;
  static const char *SETTINGS_FSX_SE_PATH;
  static const char *SETTINGS_P3D_V2_PATH;
  static const char *SETTINGS_P3D_V3_PATH;

  /* Paths for non Windows systems - used for development and debugging purposes */
  static const char *FSX_NO_WINDOWS_PATH;
  static const char *FSX_SE_NO_WINDOWS_PATH;
  static const char *P3D_V2_NO_WINDOWS_PATH;
  static const char *P3D_V3_NO_WINDOWS_PATH;

  static QString settingsKey(SimulatorType type);
  static QString registryPath(SimulatorType type);
  static QString registryKey(SimulatorType type);

  static QString documentsDirectory(QString simBasePath);
  static QString nonWindowsPath(SimulatorType type);

};

} /* namespace fs */
} /* namespace atools */

#endif // FSPATHS_H