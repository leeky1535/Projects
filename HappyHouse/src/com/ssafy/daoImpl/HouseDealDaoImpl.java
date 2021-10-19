package com.ssafy.daoImpl;

import java.sql.*;
import java.util.ArrayList;

import com.ssafy.dao.HouseDealDao;
import com.ssafy.dto.HouseDealDto;
import com.ssafy.util.DBUtil;

public class HouseDealDaoImpl implements HouseDealDao {
	private static HouseDealDao houseDealDao = new HouseDealDaoImpl();
	private DBUtil dbUtil;

	private HouseDealDaoImpl() {
		dbUtil = DBUtil.getInstance();
	}

	public static HouseDealDao getHouseDealDao() {
		return houseDealDao;
	}

	@Override
	public void register(HouseDealDto houseDealDto) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		try {
			conn = dbUtil.getConnection();
			StringBuilder sql = new StringBuilder();
			sql.append(
					"insert into housedeal (no, dong, apt_name, code, deal_amount, build_year, deal_year, deal_month, deal_day, area, floor, jibun, type, rent_money)\n"
							+ "values (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
			pstmt = conn.prepareStatement(sql.toString());
			pstmt.setInt(1, houseDealDto.getNo());
			pstmt.setString(2, houseDealDto.getDong());
			pstmt.setString(3, houseDealDto.getAptName());
			pstmt.setString(4, houseDealDto.getDongCode());
			pstmt.setString(5, houseDealDto.getDealAmount());
			pstmt.setString(6, houseDealDto.getBuildYear());
			pstmt.setString(7, houseDealDto.getDealYear());
			pstmt.setString(8, houseDealDto.getDealMonth());
			pstmt.setString(9, houseDealDto.getDealDay());
			pstmt.setString(10, houseDealDto.getArea());
			pstmt.setString(11, houseDealDto.getFloor());
			pstmt.setString(12, houseDealDto.getJibun());
			pstmt.setString(13, houseDealDto.getType());
			pstmt.setString(14, houseDealDto.getRentMoney());

			int cnt = pstmt.executeUpdate();
			System.out.println(cnt + "개 삽입성공!!!");
		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			try {
				if (pstmt != null)
					pstmt.close();
				if (conn != null)
					conn.close();
			} catch (SQLException e) {
				e.printStackTrace();
			}
		}
	}

	@Override
	public ArrayList<HouseDealDto> searchByName(String dongCode, String name) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		ArrayList<HouseDealDto> houseDealDtoList = new ArrayList<HouseDealDto>();
		try {
			conn = dbUtil.getConnection();
			String sql = "select *\n" + "from housedeal" + " where dong_code = ? and apt_name = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, dongCode);
			pstmt.setString(2, name);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				HouseDealDto houseDealDto = new HouseDealDto(rs.getInt("no"), rs.getString("dong_code"),
						rs.getString("sido"), rs.getString("gugun"), rs.getString("dong"), rs.getString("apt_name"),
						rs.getString("deal_amount"), rs.getString("build_year"), rs.getString("deal_year"),
						rs.getString("deal_month"), rs.getString("deal_day"), rs.getString("area"),
						rs.getString("floor"), rs.getString("jibun"), rs.getString("type"), rs.getString("rent_money"),
						rs.getString("lat"), rs.getString("lng"));

				houseDealDtoList.add(houseDealDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return houseDealDtoList;
	}

	@Override
	public ArrayList<HouseDealDto> searchByDong(String sido, String gugun, String dong) throws Exception {
		Connection conn = null;
		PreparedStatement pstmt = null;
		ResultSet rs = null;
		String dongCode = null;
		ArrayList<HouseDealDto> houseDealDtoList = new ArrayList<HouseDealDto>();
		try {
			conn = dbUtil.getConnection();
			String sql = "select *\n" + "from dongcode" + " where city = ? and gugun = ? and dong = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, sido);
			pstmt.setString(2, gugun);
			pstmt.setString(3, dong);
			rs = pstmt.executeQuery();
			rs.next();
			dongCode = rs.getString("code");

		} catch (SQLException e) {
			e.printStackTrace();
			return null;
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		try {
			conn = dbUtil.getConnection();
			String sql = "select *\n" + "from housedeal" + " where dong_code = ?";
			pstmt = conn.prepareStatement(sql);
			pstmt.setString(1, dongCode);
			rs = pstmt.executeQuery();
			while (rs.next()) {
				HouseDealDto houseDealDto = new HouseDealDto(rs.getInt("no"), rs.getString("dong_code"),
						rs.getString("sido"), rs.getString("gugun"), rs.getString("dong"), rs.getString("apt_name"),
						rs.getString("deal_amount"), rs.getString("build_year"), rs.getString("deal_year"),
						rs.getString("deal_month"), rs.getString("deal_day"), rs.getString("area"),
						rs.getString("floor"), rs.getString("jibun"), rs.getString("type"), rs.getString("rent_money"),
						rs.getString("lat"), rs.getString("lng"));

				houseDealDtoList.add(houseDealDto);
			}

		} catch (SQLException e) {
			e.printStackTrace();
		} finally {
			dbUtil.close(rs, pstmt, conn);
		}
		return houseDealDtoList;
	}
//
//	@Override
//	public List<HouseDealDto> searchAll() {
//		Connection conn = null;
//		PreparedStatement pstmt = null;
//		ResultSet rs = null;
//		List<HouseDealDto> houseDealDtoList = new ArrayList<HouseDealDto>();
//		try {
//			conn = dbUtil.getConnection();
//			String sql = "select no, dong, AptName, code, dealAmount, buildYear, dealYear, dealMonth, dealDay, area, floor, jibun, type, rent_money\n"
//					+ "from housedeal";
//			pstmt = conn.prepareStatement(sql);
//
//			rs = pstmt.executeQuery();
//			while (rs.next()) {
//				HouseDealDto houseDealDto = new HouseDealDto(rs.getInt("no"), rs.getString("dong"),
//						rs.getString("AptName"), rs.getString("code"), rs.getString("dealAmount"),
//						rs.getString("buildYear"), rs.getString("dealYear"), rs.getString("dealMonth"),
//						rs.getString("dealDay"), rs.getString("area"), rs.getString("floor"), rs.getString("jibun"),
//						rs.getString("type"), rs.getString("rentMoney"));
//
//				houseDealDtoList.add(houseDealDto);
//			}
//
//		} catch (SQLException e) {
//			e.printStackTrace();
//		} finally {
//			dbUtil.close(rs, pstmt, conn);
//		}
//		return houseDealDtoList;
//	}
//
//	@Override
//	public void updateDealAmount(int houseNo, String houseDealAmount) {
//		Connection conn = null;
//		PreparedStatement pstmt = null;
//		try {
//			conn = dbUtil.getConnection();
//			StringBuilder sql = new StringBuilder();
//			sql.append("update housedeal\n" + "set dealAmount = ?\n" + "where no = ?");
//			pstmt = conn.prepareStatement(sql.toString());
//
//			pstmt.setString(1, houseDealAmount);
//			pstmt.setInt(2, houseNo);
//			pstmt.executeUpdate();
//		} catch (SQLException e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				if (pstmt != null)
//					pstmt.close();
//				if (conn != null)
//					conn.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//
//	}
//
//	@Override
//	public void deleteHouse(int houseNo) {
//		Connection conn = null;
//		PreparedStatement pstmt = null;
//		try {
//			conn = dbUtil.getConnection();
//			StringBuilder sql = new StringBuilder();
//			sql.append("delete from housedeal\n" + "where no = ?");
//			pstmt = conn.prepareStatement(sql.toString());
//
//			pstmt.setInt(1, houseNo);
//			pstmt.executeUpdate();
//			System.out.println("삭제 성공!!!");
//		} catch (SQLException e) {
//			e.printStackTrace();
//		} finally {
//			try {
//				if (pstmt != null)
//					pstmt.close();
//				if (conn != null)
//					conn.close();
//			} catch (SQLException e) {
//				e.printStackTrace();
//			}
//		}
//
//	}

}