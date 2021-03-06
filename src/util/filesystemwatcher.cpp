/*****************************************************************************
* Copyright 2015-2018 Alexander Barthel albar965@mailbox.org
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

#include "util/filesystemwatcher.h"

#include <QFileInfo>
#include <QRegularExpression>
#include <QTextStream>
#include <QDebug>
#include <QFileSystemWatcher>

namespace atools {
namespace util {

FileSystemWatcher::FileSystemWatcher(QObject *parent, bool verboseLogging)
  : QObject(parent), verbose(verboseLogging)
{
  qDebug() << Q_FUNC_INFO;

  delayTimer.setSingleShot(true);
  delayTimer.connect(&delayTimer, &QTimer::timeout, this, &FileSystemWatcher::fileUpdatedDelayed);
}

FileSystemWatcher::~FileSystemWatcher()
{
  qDebug() << Q_FUNC_INFO;

  clear();
}

void FileSystemWatcher::clear()
{
  deleteFsWatcher();
  filename.clear();
}

/* Called on directory or file change and QTimer event */
void FileSystemWatcher::pathOrFileChanged()
{
  if(verbose)
    qDebug() << Q_FUNC_INFO;

  periodicCheckTimer.stop();

  QFileInfo fileinfo(filename);
  if(fileinfo.exists() && fileinfo.isFile() && fileinfo.size() > minFileSize)
  {
    if(verbose)
      qDebug() << Q_FUNC_INFO << "File" << filename
               << "exists" << fileinfo.exists()
               << "size" << fileinfo.size()
               << "last modified" << fileinfo.lastModified().toString(Qt::DefaultLocaleShortDate);

    // File exists - first call or older than two minutes or file differs
    if(!fileTimestamp.isValid() || fileinfo.lastModified() > fileTimestamp || lastFileSize != fileinfo.size())
    {
      // Timestamp of file has changed
      if(verbose)
        qDebug() << Q_FUNC_INFO << "changed" << filename;

      fileTimestamp = fileinfo.lastModified();
      lastFileSize = fileinfo.size();

      // Start or extend the delayed notification
      delayTimer.start(delayMs);
    }
    else
    {
      if(verbose)
        qDebug() << Q_FUNC_INFO << "File" << filename << "not changed";
      periodicCheckTimer.start(checkMs);
    }
  }
  else
  {
    // File was deleted - keep current weather information
    qDebug() << Q_FUNC_INFO << "File" << filename << "does not exist.";
    periodicCheckTimer.start(checkMs);
  }
}

void FileSystemWatcher::fileUpdatedDelayed()
{
  if(verbose)
    qDebug() << Q_FUNC_INFO;

  emit fileUpdated(filename);

  periodicCheckTimer.start(checkMs);
}

void FileSystemWatcher::setFilenameAndStart(const QString& value)
{
  qDebug() << Q_FUNC_INFO << value;

  clear();
  filename = value;
  createFsWatcher();
}

void FileSystemWatcher::deleteFsWatcher()
{
  delayTimer.stop();
  periodicCheckTimer.stop();
  periodicCheckTimer.disconnect(&periodicCheckTimer, &QTimer::timeout, this, &FileSystemWatcher::pathOrFileChanged);

  if(fsWatcher != nullptr)
  {
    fsWatcher->disconnect(fsWatcher, &QFileSystemWatcher::fileChanged, this, &FileSystemWatcher::pathOrFileChanged);
    fsWatcher->disconnect(fsWatcher, &QFileSystemWatcher::directoryChanged, this,
                          &FileSystemWatcher::pathOrFileChanged);
    fsWatcher->deleteLater();
    fsWatcher = nullptr;
  }
}

void FileSystemWatcher::createFsWatcher()
{
  if(fsWatcher == nullptr)
  {
    // Watch file for changes and directory too to catch file deletions
    fsWatcher = new QFileSystemWatcher(this);
    fsWatcher->connect(fsWatcher, &QFileSystemWatcher::fileChanged, this, &FileSystemWatcher::pathOrFileChanged);
    fsWatcher->connect(fsWatcher, &QFileSystemWatcher::directoryChanged, this, &FileSystemWatcher::pathOrFileChanged);
  }

  // Watch file to get changes
  if(!fsWatcher->addPath(filename))
    qWarning() << "cannot watch" << filename;

  // Watch directory to get added or removed file changes
  QFileInfo fileinfo(filename);
  if(!fsWatcher->addPath(fileinfo.path()))
    qWarning() << "cannot watch" << fileinfo.path();

  // Check every ten seconds since the watcher is unreliable
  periodicCheckTimer.connect(&periodicCheckTimer, &QTimer::timeout, this, &FileSystemWatcher::pathOrFileChanged);
  periodicCheckTimer.start(checkMs);
}

} // namespace util
} // namespace atools
