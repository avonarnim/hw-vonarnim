#include <stdexcept>
#include <algorithm>
#include <iomanip>
#include "streetgrid.h"
#include "derived_events.h"

StreetGrid::StreetGrid(
    const std::vector<unsigned int> rowCapacities,
    const std::vector<unsigned int> colCapacities)
    : rowCapacities_(rowCapacities), colCapacities_(colCapacities)
{
    n = rowCapacities.size();
    m = colCapacities.size();
    expectedVehicles_ = 0;

    // TO BE COMPLETED
    rowGrid = new int**[n];
    for (int i = 0; i < n; ++i)
    { rowGrid[i] = new int*[m];
      for (int j = 0; j < m; ++j)
      {
        rowGrid[i][j] = new int[2];
        rowGrid[i][j][0] = 0;
        rowGrid[i][j][1] = 0;
      }
    }

    colGrid = new int**[n];
    for (int i = 0; i < n; ++i)
    { colGrid[i] = new int*[m];
      for (int j = 0; j < m; ++j)
      {
        colGrid[i][j] = new int[2];
        colGrid[i][j][0] = 0;
        colGrid[i][j][1] = 0;
      }
    }

    // Call monitor state to output starting state
    monitorState();
}

StreetGrid::~StreetGrid()
{
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < m; ++j)
      delete [] rowGrid[i][j];
    delete [] rowGrid[i];
  }
  delete [] rowGrid;
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < m; ++j)
      delete [] colGrid[i][j];
    delete [] colGrid[i];
  }
  delete [] colGrid;
}

size_t StreetGrid::numRows() const
{
    return n;
}
size_t StreetGrid::numCols() const
{
    return m;
}
void StreetGrid::setExpected(size_t expectedVehicles)
{
    expectedVehicles_ = expectedVehicles;
}

void StreetGrid::outputCompletedVehicles(std::ostream& ostr) const
{
    for(auto v : completed_) {
        ostr << v.end << " " << v.id << std::endl;
    }
}

bool StreetGrid::allVehiclesArrived() const
{
    return completed_.size() == expectedVehicles_;
}

void StreetGrid::monitorState()
{
  std::cout << "expected vehicles: " << expectedVehicles_ << " , completed vehicles: " << completed_.size() << std::endl;
    std::cout << "in monitorState" << std::endl;
    std::cout << "State: " << std::endl;
    std::cout << std::setw(4) << " ";

    // Print header row with column capacities
    for(size_t cidx = 0; cidx < m; ++cidx) {
        std::cout <<  std::setw(4) << colCapacities_[cidx] << " " <<  std::setw(4) << " ";
    }
    std::cout << std::endl;
    std::cout << "    ";
    for(size_t cidx = 0; cidx < m; ++cidx) {
        std::cout <<  std::setw(4) << "====="  << "=====";
    }
    std::cout << std::endl;

    // Start printing grid data
    for(size_t ridx = 0; ridx < n; ++ridx) {
        // Print row capacity
        std::cout << std::setw(2) << rowCapacities_[ridx] << " |";
        // Print row occupancies
        for(size_t cidx = 0; cidx < m; ++cidx) {
            std::cout << std::setw(4) << "+" << " ";
            if(cidx < m-1) {
                if(isBlocked(ridx,cidx,true)) {
                    std::cout << std::setw(3) << getRowOccupancy(ridx,cidx);
                    std::cout << "B";
                }
                else {
                    std::cout << std::setw(4) << getRowOccupancy(ridx,cidx);
                }
            }
        }
        std::cout << std::endl;
        std::cout <<  "   |";
        // Print column occupancies
        if(ridx < n-1) {
            for(size_t cidx = 0; cidx < m; ++cidx) {
                if(isBlocked(ridx,cidx,false)) {
                    std::cout << std::setw(3) << getColOccupancy(ridx,cidx);
                    std::cout << "B";
                }
                else {
                    std::cout << std::setw(4) << getColOccupancy(ridx,cidx);
                }
                std::cout << std::setw(4) << " " << " " ;
            }
        }
        std::cout << std::endl;

    }
}

/**
 * [TO BE COMPLETED]
 * Returns the number of vehicles on the row street segment originating
 * from the intersection given by row,col
 */
size_t StreetGrid::getRowOccupancy(size_t row, size_t col) const
{
  return rowGrid[row][col][0];
}

/**
 * [TO BE COMPLETED]
 * Returns the number of vehicles on the column street segment originating
 * from the intersection given by row,col
 */
size_t StreetGrid::getColOccupancy(size_t row, size_t col) const
{
    return colGrid[row][col][0];
}

/**
 * [TO BE COMPLETED]
 * Returns true if the street segment is blocked (false otherwise) for
 * the row street segment (if rowDir=true) or column street segment
 * (if rowDir=false) that originates from the intersection given
 * by row,col
 */
bool StreetGrid::isBlocked(size_t row, size_t col, bool rowDir) const
{
    if (rowDir)
      return rowGrid[row][col][1] == 1;
    //if asking about the column
    return colGrid[row][col][1] == 1;
}

/**
 * [TO BE COMPLETED]
 * Returns true if the vehicle at intersection row,col should choose
 *  to travel down a row or travel down a column
 *
 * If the vehicle can choose either, it should choose the option
 *  with the lower time/delay. If both directions have an equal time
 *  then vehicles should choose the opposite direction as the last vehicle
 *  in that intersection that encountered equal times in both directions,
 *  starting with the choice of row for the first vehicle in that situation.
 *
 * @returns true if the vehicle should travel in the current row and
 *          false if the vehicle should travel down the current column
 */
bool StreetGrid::shouldChooseRow(size_t row, size_t col)
{
    std::cout << "In shouldChooseRow. Vehicle position: " << row << " " << col << std::endl;
    if (isBlocked(row, col, true))
    {
      std::cout << "row blocked" << std::endl;
      return false;
    }
    else if (isBlocked(row, col, false))
    {
      std::cout << "col blocked" << std::endl;
      return true;
    }
    if (row == n-1) //if the vehicle is in the bottom row, it must go across the row
      return true;
    if (col == m-1) //if the vehicle is in the rightmost column, it must go down the column
      return false;
    int rowTime = max(1, (double)(1+rowGrid[row][col][0]/rowCapacities_[row])*SCALE_FACTOR);
    int colTime = max(1, (double)(1+colGrid[row][col][0]/colCapacities_[col])*SCALE_FACTOR);
    if (rowTime < colTime)  //traverse this row
      return true;
    else if (rowTime > colTime) //traverse this column
      return false;
    else
    {
      std::map<std::pair<int, int> , bool>::iterator it = encountersEqual.find(std::make_pair(row, col));
      if (it != encountersEqual.end())
      {
        if (it->second == true) //last one was row
          {
            it->second = false;
            return false;
          }
        else
        {
          it->second = true;
          return true;
        }
      }
      else
      {
        encountersEqual.insert(std::make_pair(std::make_pair(row, col), true));
        return true;
      }
    }
}
//returns the max value between two parameters
double StreetGrid::max(double l, double r)
{
  if (l > r)
    return l;
  else
    return r;
}


/**
 * [TO BE COMPLETED]
 * Blocks or unblocks a particular street segment
 *
 * @pre Both row and column segments from a given intersection/vertex
 *      shall not be blocked at the same time. Neither should a row
 *      row segment in the bottom nor a column sigment in the rightmost
 *      column
 *
 * @param[in] row, col
 *    Row and column of starting vertex of the street segment
 * @param[in] rowDir
 *    True indicates the row segment should be set,
 *    false indicates the column segment should be set
 * @param[in] val
 *    True indicates blocked, false indicates unblocked
 */
void StreetGrid::setBlockage(size_t row, size_t col, bool rowDir, bool val)
{
  if (row < 0 || col < 0)
    return;
  if (val)
  {
    std::cout << "setting block" << std::endl;
    if (rowDir)
    {
      if (row >= n || col >= m-1)
        return;
      else
        rowGrid[row][col][1] = 1;
    }
    else
    {
      if (row >= n-1 || col >= m)
        return;
      else
        colGrid[row][col][1] = 1;
    }
  }
  else
  {
    std::cout << "removing block" << std::endl;
    if (rowDir)
    {
      if (row >= n || col >= m-1)
        return;
      else
        rowGrid[row][col][1] = 0;
    }
    else
    {
      if (row >= n-1 || col >= m)
        return;
      else
        colGrid[row][col][1] = 0;
    }
  }
}

/**
 * [TO BE COMPLETED]
 * Processes a vehicle arrival and moves it to the next street segment
 *
 * @param[in] vehicleID
 *  ID of the vehicle to process
 * @param[in] ts
 *  Now (current time)
 * @return EventList of new events spawned by this processing
 *
 * @throws std::logic_error if no vehicle with that ID exists
 */
EventList StreetGrid::processArrival(const std::string& vehicleID, const TimeStamp& ts)
{
    EventList consequences;
    std::map<std::string, Vehicle>::iterator it = vehicles_.find(vehicleID);
    if (it == vehicles_.end())
      throw std::logic_error("No vehicle with that ID exists");

    std::cout << "In processArrival. Vehicle position: " << it->second.rowIndex << " " << it->second.colIndex << std::endl;
    if (it->second.rowIndex == n-1 && it->second.colIndex == m-1)
    {
      if (it->second.rowDir)
      {
        rowGrid[it->second.rowIndex][it->second.colIndex-1][0] -= 1;
      }
      else
      {
        colGrid[it->second.rowIndex-1][it->second.colIndex][0] -= 1;
      }
      completed_.push_back(it->second);
      vehicles_.erase(it);
    }
    else
    {
      if (shouldChooseRow(it->second.rowIndex, it->second.colIndex))
      {
        std::cout << "traversing across row" << std::endl;
        if (it->second.rowDir)
        {
          rowGrid[it->second.rowIndex][it->second.colIndex-1][0] -= 1;
        }
        else
        {
          colGrid[it->second.rowIndex-1][it->second.colIndex][0] -= 1;
        }
        it->second.end += max(1, (double)(1+rowGrid[it->second.rowIndex][it->second.colIndex][0])/rowCapacities_[it->second.rowIndex])*SCALE_FACTOR;
        rowGrid[it->second.rowIndex][it->second.colIndex][0] += 1;
        it->second.colIndex += 1;
        it->second.rowDir = true;
        consequences.push_back(new ArrivalEvent(it->second.end, *this, vehicleID));
      }
      else
      {
        std::cout << "traversing down column" << std::endl;
        if (it->second.rowDir)
        {
          rowGrid[it->second.rowIndex][it->second.colIndex-1][0] -= 1;
        }
        else
        {
          colGrid[it->second.rowIndex-1][it->second.colIndex][0] -= 1;
        }
        it->second.end += max(1, (double)(1+colGrid[it->second.rowIndex][it->second.colIndex][0])/colCapacities_[it->second.colIndex])*SCALE_FACTOR;
        colGrid[it->second.rowIndex][it->second.colIndex][0] += 1;
        it->second.rowIndex += 1;
        it->second.rowDir = false;
        consequences.push_back(new ArrivalEvent(it->second.end, *this, vehicleID));
      }
    }

    return consequences;
}

/**
 * [TO BE COMPLETED]
 * Adds a new vehicle to the street grid using
 * assuming its start TimeStamp represents the current time.
 *
 * @param[in] v
 *    Vehicle data
 *
 * @return EventList of new events spawned by this addition
 *
 * @throws std::logic_error if a vehicle with that ID has already
 *           been added
 */
EventList StreetGrid::addVehicle(const Vehicle& v)
{
    std::cout << "In addVehicle. Vehicle position: " << v.id << v.rowIndex << " " << v.colIndex << std::endl;
    EventList consequences;
    std::map<std::string, Vehicle>::iterator it = vehicles_.find(v.id);
    if (it != vehicles_.end())
      throw std::logic_error("Vehicle with this ID has already been added");
    vehicles_.insert(std::make_pair(v.id, v));
    it = vehicles_.find(v.id);

    if (v.rowIndex == n-1 && v.colIndex == m-1)
    {
      completed_.push_back(it->second);
      vehicles_.erase(it);
    }
    else
    {
      std::cout << "should check if choosing row" << std::endl;
      if (shouldChooseRow(v.rowIndex, v.colIndex))
      {
        std::cout << "traversing across row" << std::endl;
        it->second.end = it->second.start + max(1, (double)(1+rowGrid[v.rowIndex][v.colIndex][0])/rowCapacities_[v.rowIndex])*SCALE_FACTOR;
        rowGrid[v.rowIndex][v.colIndex][0] += 1;
        it->second.colIndex = v.colIndex + 1;
        it->second.rowDir = true;
      }
      else
      {
        std::cout << "traversing down col" << std::endl;
        it->second.end = it->second.start + max(1, (double)(1+colGrid[v.rowIndex][v.colIndex][0])/colCapacities_[v.colIndex])*SCALE_FACTOR;
        colGrid[v.rowIndex][v.colIndex][0] += 1;
        it->second.rowIndex = v.rowIndex + 1;
        it->second.rowDir = false;
      }
      consequences.push_back(new ArrivalEvent(it->second.end, *this, v.id));
    }
    std::cout << "Exiting addVehicle. Vehicle position: " << it->second.rowIndex << " " << it->second.colIndex << std::endl;
    return consequences;
}
