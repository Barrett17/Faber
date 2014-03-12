/*
    Copyright 2013 Dario Casalinuovo. All rights reserved.

    This file is part of Faber.

    Faber is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Faber is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Faber.  If not, see <http://www.gnu.org/licenses/>.
*/


class SnapshotManager {
public:
									SnapshotManager();
	virtual							~SnapshotManager();

	uint32							GetNextRevision(uint32 rev);

	uint32							CurrentRevision();

	status_t						RestoreStatus(uint32 rev);

	status_t						FreezeRequested(uint32* newRev) const;

private:

};
